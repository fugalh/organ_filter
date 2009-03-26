organ_filter: organ_filter.c
	 gcc -o $@ `pkg-config --libs alsa` $<
