
extern void FLASH_WriteProtection(void);
/*---------------------------------------------------------------------------------------------------------*/
/* MAIN function                                                                                           */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
	FLASH_WriteProtection( );
	while(1);
	return (0);

}
