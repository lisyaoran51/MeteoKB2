#ifndef SDP_H
#define SDP_H

#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

sdp_session_t * sdp_register_service(int port);
int sdp_init_server(int port);

#ifdef __cplusplus
}
#endif


#endif