#include <stdio.h>
#include <stdlib.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <bluetooth/rfcomm.h>


int browse(char *dest)
{
        uint32_t svc_uuid_int[] = { 0, 0, 0, 0xABCD };
        int status;
        bdaddr_t target;
        uuid_t svc_uuid;
        sdp_list_t *response_list, *search_list, *attrid_list;
        sdp_session_t *session = 0;
        uint32_t range = 0x0000ffff;
        uint8_t port = 0;
        //if(argc < 2)
        //{
        //        fprintf(stderr, "usage: %s <bt_addr>\n", argv[0]);
        //        exit(2);
        //}
        str2ba( dest, &target );

        // connect to the SDP server running on the remote machine
        session = sdp_connect( BDADDR_ANY, &target, 0 );
        sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
        search_list = sdp_list_append( 0, &svc_uuid );
        attrid_list = sdp_list_append( 0, &range );
        // get a list of service records that have UUID 0xabcd
        response_list = NULL;



        status = sdp_service_search_attr_req( session, search_list, \
        SDP_ATTR_REQ_RANGE, attrid_list, &response_list);
        if( status == 0 ) {
                sdp_list_t *proto_list;
                sdp_list_t *r = response_list;
                // go through each of the service records
                for (; r; r = r->next ) {
                        sdp_record_t *rec = (sdp_record_t*) r->data;
                        // get a list of the protocol sequences
                        if( sdp_get_access_protos( rec, &proto_list ) == 0 ) {
                                // get the RFCOMM port number
                                port = sdp_get_proto_port( proto_list, RFCOMM_UUID );
                                sdp_list_free( proto_list, 0 );
                        }
                sdp_record_free( rec );
                }
        }
        sdp_list_free( response_list, 0 );
        sdp_list_free( search_list, 0 );
        sdp_list_free( attrid_list, 0 );
        sdp_close( session );

        return port;



        if( port != 0 ) {
                printf("found service running on RFCOMM port %d\n", port);
        }
        return 0;
}

int init_connection(char* dest, int port){

        struct sockaddr_rc addr = { 0 };
        int s, status;
        //char dest[18] = "01:23:45:67:89:AB";

        // allocate a socket
        s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // set the connection parameters (who to connect to)
        addr.rc_family = AF_BLUETOOTH;
        addr.rc_channel = (uint8_t) port;
        str2ba( dest, &addr.rc_bdaddr );

        // connect to server
        status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

        // send a message
        if( status == 0 ) {
                return s;
                status = write(s, "hello!", 6);
        }

        if( status < 0 ) {
                perror("uh oh");
                return s;
        }
}

int main(int argc, char** argv){

        int port = browse(argv[1]);
        int server = init_connection(argv[1], port);
        int status;

        while(1){
                status = write(server, "hello!", 6);
                sleep(1);
        }

        close(server);

        return 0;

}
