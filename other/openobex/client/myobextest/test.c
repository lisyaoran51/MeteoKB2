// gcc test.c -o -lopenobex -lbluetooth


/**
	\file apps/obex_test.c
	Test IrOBEX, TCPOBEX and OBEX over cable to R320s.
	OpenOBEX test applications and sample code.

	Copyright (c) 2000 Pontus Fuchs, All Rights Reserved.

	OpenOBEX is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as
	published by the Free Software Foundation; either version 2 of
	the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public
	License along with OpenOBEX. If not, see <http://www.gnu.org/>.
 */

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "openobex/obex.h"

#if defined(_WIN32)
#define in_addr_t unsigned long

#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "obex_test.h"
#include "obex_test_server.h"
#include "obex_test_client.h"

#define TRUE  1
#define FALSE 0

#define IR_SERVICE "OBEX"
#define BT_CHANNEL 4

//
// Called by the obex-layer when some event occurs.
//
static void obex_event(obex_t *handle, obex_object_t *object, int mode,
					int event, int obex_cmd, int obex_rsp)
{
	printf("my eventcb... %d\n", event);
	switch (event)	{
	case OBEX_EV_PROGRESS:
		printf("Made some progress...\n");
		break;

	case OBEX_EV_ABORT:
		printf("Request aborted!\n");
		break;

	case OBEX_EV_REQDONE:
		if (mode == OBEX_MODE_CLIENT) {
			client_done(handle, object, obex_cmd, obex_rsp);
		}
		else	{
			server_done(handle, object, obex_cmd, obex_rsp);
		}
		break;
	case OBEX_EV_REQHINT:
		/* Accept any command. Not rellay good, but this is a test-program :) */
		OBEX_ObjectSetRsp(object, OBEX_RSP_CONTINUE, OBEX_RSP_SUCCESS);
		break;

	case OBEX_EV_REQ:
		server_request(handle, object, event, obex_cmd);
		break;

	case OBEX_EV_LINKERR:
		OBEX_TransportDisconnect(handle);
		printf("Link broken!\n");
		break;

	case OBEX_EV_STREAMEMPTY:
		fillstream(handle, object);
		break;

	default:
		printf("Unknown event %02x!\n", event);
		break;
	}
}

/*
struct context
{
	int serverdone;
	int clientdone;
	char *get_name;	// Name of last get-request 
};
*/

/*
//
// Wait for an obex command to finish.
//
static void syncwait(obex_t *handle)
{
	struct context *gt;
	int ret;
	
	gt = OBEX_GetUserData(handle);
	printf("got user data\n");
	while(!gt->clientdone) {
		//printf("syncwait()\n");
		
		printf("try handle input...");
		ret = OBEX_HandleInput(handle, 10);
		printf("done\n");
		if(ret < 0) {
			printf("Error while doing OBEX_HandleInput()\n");
			break;
		}
	}

	gt->clientdone = FALSE;
}
*/
	

#define BDADDR_TO   (&(bdaddr_t) {{0xB8, 0x27, 0xEB, 0xE3, 0x6A, 0x6A}})
//
//
//
int main (int argc, char *argv[])
{
	obex_t *handle = NULL;
	struct context global_context = {0};
	
	int btobex = FALSE;
	bdaddr_t bdaddr;
	uint8_t channel = 4;
	unsigned int flags = 0;
	
	char dest[18] = "B8:27:EB:E3:6A:6A";
	str2ba( dest, &bdaddr );
	
	handle = OBEX_Init(OBEX_TRANS_BLUETOOTH, obex_event, flags);
	printf("Init done\n");
	//bacpy(&bdaddr, BDADDR_TO);
	
	OBEX_SetUserData(handle, &global_context);
	
	if (BtOBEX_TransportConnect(handle, BDADDR_ANY, &bdaddr, channel) <0) {
		printf("Transport connect error! (Bluetooth)\n");
	}
	printf("connect done\n");
	// connect_client
	
	obex_object_t *object;
	obex_headerdata_t hd;
	int err;

	object = OBEX_ObjectNew(handle, OBEX_CMD_CONNECT);
	if(object == NULL) {
		printf("Error\n");
		return;
	}
	printf("new object done\n");
	hd.bs = (uint8_t *) "Linux";
	if(OBEX_ObjectAddHeader(handle, object, OBEX_HDR_WHO, hd, 6,
				OBEX_FL_FIT_ONE_PACKET) < 0)
	{
		printf("Error adding header\n");
		OBEX_ObjectDelete(handle, object);
		return;
	}
	printf("AddHeader done\n");
	err = OBEX_Request(handle, object);
	if (err) {
		OBEX_ObjectDelete(handle, object);
		printf("Error: %s\n", strerror(err));
		return;
	}
	printf("request done\n");
	syncwait(handle);
	
	
	push_client(handle);
	
	while(1);
	
	return 0;
}


int read_input(char *answer, size_t size, const char *question, ...)
{
	va_list ap;
	va_start(ap, question);
	vfprintf(stdout, question, ap);
	va_end(ap);

	fflush(stdout);
	if (fgets(answer, size, stdin) == NULL)
		return -1;

	answer[strlen(answer)-1] = '\0'; /* remove trailing newline */
	return strlen(answer);
}	
