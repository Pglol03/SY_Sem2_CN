#define MAX_SEQ 1
typedef enum {frame_arrival,cksum_err,timeout}event_type;
#include ""

void send3(void){
    seq_nr next_frame_to_send;
    frame s;
    packet buffer;
    event_type event;

    next_frame_to_send = 0;
    from_network_layer(&buffer);
    while (TRUE)
    {
        s.info = buffer;
        s.seq = next_frame_to_send;
        to_physical_layer(&s);
        start_timer(s.seq);
        wait_for_event(&event);
        if(event == frame_arrival){
            from_physical_layer(&s);
            if(s.ack == next_frame_to_send){
                stop_timer(s.ack);
                from_network_layer(&buffer);
                inc(next_frame_to_send);
            }
        }
    }
}