typedef struct Multiboot Multiboot;

int kmain(__attribute__((__unused__)) Multiboot* mboot_ptr)
{
	return 0xDEADBABA;
}
