#include <drvmgr/leon2_amba_bus.h>

#ifdef CONFIGURE_DRIVER_LEON2_AT697PCI

/* AT697 PCI Driver resources. Connects a System IRQ with a PCI IRQ */
struct drvmgr_key leon2_amba_at697pci_res[] =
{
	/* PCI IRQ              LEON2 SYSTEM IRQ */
	{"INTA#", KEY_TYPE_INT, {(unsigned int)5}},
	{"INTB#", KEY_TYPE_INT, {(unsigned int)5}},
	{"INTC#", KEY_TYPE_INT, {(unsigned int)5}},
	{"INTD#", KEY_TYPE_INT, {(unsigned int)5}},
	/* PCI IRQ                  LEON2 PIO PIN THAT GENERATE IRQ */
	{"INTA#_PIO", KEY_TYPE_INT, {(unsigned int)7}},
	{"INTB#_PIO", KEY_TYPE_INT, {(unsigned int)7}},
	{"INTC#_PIO", KEY_TYPE_INT, {(unsigned int)7}},
	{"INTD#_PIO", KEY_TYPE_INT, {(unsigned int)7}},
	/* Setup PCI BARs to be 32MB continous */
	{"tgtbar1", KEY_TYPE_INT, {(unsigned int)0x40000000}},
	{"tgtbar2", KEY_TYPE_INT, {(unsigned int)0x41000000}},
	KEY_EMPTY
};

/* Defines the At697 PCI core location and IRQ */
struct drvmgr_key leon2_custom_at697[] =
{
	{"REG0", KEY_TYPE_INT, {0x80000100}},
	{"IRQ0", KEY_TYPE_INT, {14}},
	KEY_EMPTY
};
#endif

/* Defines what's on the bus. */
struct leon2_core leon2_amba_custom_cores[] = 
{
#ifdef CONFIGURE_DRIVER_LEON2_AT697PCI
	{{LEON2_AMBA_AT697PCI_ID}, "AT697_PCI", &leon2_custom_at697[0]},
#endif
	EMPTY_LEON2_CORE
};

#ifndef RTEMS_DRVMGR_STARTUP
/* Configure LEON2 Root bus driver */
struct leon2_bus leon2_bus_config = 
{
	&leon2_std_cores[0],		/* The standard cores, defined by driver */
	&leon2_amba_custom_cores[0],	/* custom cores, defined by us */
	DRVMGR_TRANSLATE_ONE2ONE,
	DRVMGR_TRANSLATE_ONE2ONE,
};
#endif

/* Driver resources on LEON2 AMBA bus */
struct drvmgr_bus_res leon2_amba_res =
{
	.next = NULL,
	.resource = {
	
#ifdef CONFIGURE_DRIVER_LEON2_AT697PCI
		{DRIVER_LEON2_AMBA_AT697PCI, 0, &leon2_amba_at697pci_res[0]},
#endif
		RES_EMPTY
	},
};

/* LEON2 specific system init */
void system_init2(void)
{
#ifndef RTEMS_DRVMGR_STARTUP
	/* Register LEON2 root bus */
	leon2_root_register(&leon2_bus_config, &leon2_amba_res);
#endif
}
