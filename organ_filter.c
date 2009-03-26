/* organ_filter.c
 * Hans Fugal <hans@fugal.net>  2004
 * gcc -o organ_filter `pkg-config --libs alsa` organ_filter.c
 */

#include <alsa/asoundlib.h>

void organ_filter(snd_seq_t *seq, int port, snd_seq_event_t *ev)
{
    while (snd_seq_event_input(seq, &ev) >= 0)
    {
	if (ev->type == SND_SEQ_EVENT_NOTEON && ev->data.note.velocity != 0)
	    ev->data.note.velocity = 64;

	snd_seq_ev_set_source(ev, port);
	snd_seq_ev_set_subs(ev);
	snd_seq_ev_set_direct(ev);
	snd_seq_event_output(seq, ev);
	snd_seq_drain_output(seq);
    }
}

snd_seq_t *open_client()
{
    snd_seq_t *handle;
    int err;
    err = snd_seq_open(&handle, "default", SND_SEQ_OPEN_DUPLEX, 0);
    if (err <0)
    {
	fprintf(stderr, "Error opening sequencer\n");
	exit(1);
    }
    snd_seq_set_client_name(handle, "Organ Filter");
    return handle;
}

int create_port(snd_seq_t *seq,char *name)
{
    int portid;
    if ((portid = snd_seq_create_simple_port(seq, name, 
		    SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE|
		    SND_SEQ_PORT_CAP_READ|SND_SEQ_PORT_CAP_SUBS_READ,
		    SND_SEQ_PORT_TYPE_APPLICATION)) < 0) 
    {
	fprintf(stderr, "Error creating port %s\n", name);
	exit(1);
    }
    return portid;
}

int main(void)
{
    snd_seq_t *seq;
    snd_seq_event_t ev;
    int port;

    seq = open_client();
    port = create_port(seq,"Organ");
    organ_filter(seq, port, &ev);
}
