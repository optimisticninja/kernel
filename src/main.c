
struct MultibootHeader;
typedef struct MultibootHeader MultibootHeader;

void kernel_early()
{
}

int kmain(__attribute__((__unused__))MultibootHeader* mboot_header) 
{
	kernel_early();

	return 666;
}
