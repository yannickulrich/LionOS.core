/*
      #     #               ###    ###
      #                    #   #  #
      #     #   ###   ##   #   #   ##
      #     #  #   #  # #  #   #     #
      ####  #   ###   # #   ###   ###
                    FAT
*/
#define FATSIZE 0x10
/*
void rewriteFAT()
{
	uint8_t buf[512];
	uint16_t akbuf;
	int i;
	for(i = 0; i < FATSIZE; i++)
	{
		if (fat1[i].free == 1)
			akbuf = 0x000;
		else if (fat1[i].defect == 1)
			akbuf = 0xFF7;
		else if (fat1[i].lastCluster == 1)
			akbuf = 0xFF8;
		else	
			akbuf = fat1[i].content+2;
	
		
	}
}
*/
FATENTRY *initFAT()
{ 
	DEBUG_MSG("FAT:      Init...");
	kmemset((void*) sysbuffer, 0x0, 0x2400);
	readSector(1); // start at 0x2600: root directory (14 sectors)
	uint8_t buf[512];
	kmemcpy( (void *) buf, (void *) sysbuffer, 512);

	int i;
	for(i = 0; i<FATSIZE; i++)
	{
		buf[i] = ( (buf[i] & 0xF) << 4) | (buf[i] >> 4);
	}
	for(i = 0; i < FATSIZE; i++)
	{
		int byte;
		unsigned int offset;
		if ( i % 2 == 0 )
		{
			//offset = i*1.5
			
			offset = (char)(i*3/2);
			byte   = buf[offset  ]<<8;
			byte  |= buf[offset+1];
			byte >>= 4;
		}
		else
		{
			offset = (char)(i*3/2);
			byte   = buf[offset  ]<<8;
			byte  |= buf[offset+1];
			byte  &= 0xFFF;
		}
		fat1[i].content = -1;
		if ( byte == 0x000 )
		{
			//Free Cluster
			fat1[i].free = true;
		}
		else if ( byte == 0xFF7 )
		{
			//Defect Cluster
			fat1[i].defect = true;
		}
		else if ( byte >= 0xFF8 && byte <= 0xFFF ) //0xFF8-0xFFF
		{
			//Last Cluster
			fat1[i].lastCluster = true;
		}
		else
		{
			fat1[i].content = byte-2;
		}
	}
	dbg(true);
	return &(fat1[0]);
}

directory *listDir( directory *root )
{
	#define FAT_OFFSET 0x20
	int sektor;
	if (root == NULL)
	{
		root = pmm_malloc(sizeof(directory));
		root->ID = 0;
		root->name[0] = '/';
		root->first = NULL;
		root->firstSub = NULL;
		sektor = ROOT_SEKTOR;
	}
	else
	{
		sektor = root->clusterID +0x21;
	}

	kmemset((void*) sysbuffer, 0x0, 0x2400);
	readSector(sektor); // start at 0x2600: root directory (14 sectors)
	
	uint8_t buf[LSDIR_BUFFER];
	kmemcpy((void*) buf, (void*) sysbuffer, LSDIR_BUFFER);


	file *akfile;
	directory *akdir;
	int i, c, entry;
	for(i = 0; i < 224; i++)       // 224 Entries * 32 Byte
	{
		if (*((char*) (sysbuffer + i * 64 + 0 + FAT_OFFSET)) == ' ' || *((char*) (sysbuffer + i * 64 + 0 + FAT_OFFSET)) == 0)
			break;
		if ( (int) *( (char *)( sysbuffer + 0x0B + i*64 + FAT_OFFSET ) ) == 0x10 ) //Is Dir? YES
		{
			if (root->firstSub == NULL)
			{
				//First file
				root->firstSub = pmm_malloc(sizeof( directory ));
				root->firstSub->next = NULL;
				akdir = root->firstSub;
			}
			else
			{
				akdir = root->firstSub;
				while( akdir->next != NULL )
					akdir = akdir->next;
				akdir->next = pmm_malloc(sizeof(directory));
				akdir->next->next = NULL;	
				akdir = akdir->next;
				
			}
			akdir->ID = i;
			
			akdir->name = pmm_malloc(8);
			for (c = 0; c < 8; c++)
			{
				if (*((char*) (sysbuffer + i * 64 + c + FAT_OFFSET )) == ' ')
					break;
				akdir->name[c] = *((char*) (sysbuffer + i*64 + c + FAT_OFFSET));
			
			}
			akdir->clusterID = ((( (int) *( (char *)( sysbuffer + 0x1A + i*64 + FAT_OFFSET ) ) ) << 8 ) | ( (int) *( (char *)( sysbuffer + 0x3A + i*64 + 1 + FAT_OFFSET ) ) ));
		
			//Example:
		
			//akfile->clusterID = 0x345 => 0x543
			
			akdir->clusterID = ( akdir->clusterID >> 8 ) | ((akdir->clusterID & 0xF0)) | ((akdir->clusterID & 0xF0) << 8) ;
			akdir->clusterID -= 2;
		}
		else
		{
			/*         
						   0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
				00002600  e5 55 4d 4d 59 20 20 20  20 20 20 20 00 64 5d 62  |.UMMY       .d]b|
				00002610  7c 3d 7c 3d 00 00 5d 62  7c 3d 00 00 00 00 00 00  ||=|=..]b|=......|
				00002620  41 4d 00 79 00 4f 00 53  00 2e 00 0f 00 55 73 00  |AM.y.O.S.....Us.|
				00002630  68 00 00 00 ff ff ff ff  ff ff 00 00 ff ff ff ff  |h...............|
				00002640  4d 59 4f 53 20 20 20 20  53 48 20 20 00 00 cf 75  |MYOS    SH  ...u|
				00002650  7c 3d 7c 3d 00 00 cf 75  7c 3d 03 00 d5 27 00 00  ||=|=...u|=...'..|
				00002660  41 54 00 45 00 53 00 54  00 31 00 0f 00 cb 2e 00  |AT.E.S.T.1......|
				00002670  74 00 78 00 74 00 00 00  ff ff 00 00 ff ff ff ff  |t.x.t...........|
				Filename : 0x2640-0x2647
				Extention: 0x2648-0x264A
				1. Ist das erste Zeichen des Namens ein 0x0E, dann ist dieser Eintrag noch frei und kann
				   benutzt werden.
				2. Ist das erste Zeichen des Namens ein 0x00, dann ist dieser UND die folgenden
				   Einträge frei und können genutzt werde. Dabei kann man sich dann im Falle des
				   Durchsuchens des Verzeichnisses sparen die nächsten Einträge ebenfalls noch
				   durchzuschauen.
			*/
			
			
		
			if (root->first == NULL)
			{
				//First file
				root->first = pmm_malloc(sizeof( file ));
				root->first->next = NULL;
				akfile = root->first;
				
			}
			else
			{
				akfile = root->first;
				while( akfile->next != NULL )
					akfile = akfile->next;
				akfile->next = pmm_malloc(sizeof(file));
				akfile->next->next = NULL;	
				akfile = akfile->next;
			}
			akfile->ID = i;
			
			for (c = 0; c < 8; c++)
			{
				if (*((char*) (sysbuffer + i * 64 + c + FAT_OFFSET )) == ' ')
					break;
				akfile->name[c] = *((char*) (sysbuffer + i*64 + c + FAT_OFFSET));
			
			}
		
			if( akfile->name[0] == 0x0E )
				continue; //Entry free
			else if( akfile->name[0] == 0x00 )
			{
				akfile->next = (file *) -1;
				return root;
				break;
			}
			for (c=0;c<=3;c++)
			{
				akfile->extension[c] = *( (char *)( sysbuffer + 0x08 + i*64 + c + FAT_OFFSET ) );
			}
			akfile->clusterID = ((( (int) *( (char *)( sysbuffer + 0x1A + i*64 + FAT_OFFSET ) ) ) << 8 ) | ( (int) *( (char *)( sysbuffer + 0x3A + i*64 + 1 + FAT_OFFSET ) ) ));
		
			/*Example:
		
			akfile->clusterID = 0x345 => 0x543
			*/
			akfile->clusterID = ( akfile->clusterID >> 8 ) | ((akfile->clusterID & 0xF0)) | ((akfile->clusterID & 0xF0) << 8) ;
			akfile->clusterID -= 2;


			//Read atributes
			/*
				Bit 0 : Read Only
				Bit 1 : Hidden
				Bit 2 : System
				Bit 3 : Volume Label
				Bit 4 : Subdirectory
				Bit 5 : Archive
				Bit 6 : Device
				Bit 7 : Unused
				*/
			int atr =  (int) *( (char *)( sysbuffer + 0x0B + i*64 + FAT_OFFSET ) );
			//uint16_t size = (int) *( (char *)( sysbuffer + 0x1C + i*64 ) );
			/*
			ATTR_READ_ONLY 0x01
			ATTR_HIDDEN 0x02
			ATTR_SYSTEM 0x04
			ATTR_VOLUME_ID 0x08
			ATTR_DIRECTORY 0x10
			ATTR_ARCHIVE 0x20
			*/
			akfile->readOnly = (atr == 0x01);
			akfile->hidden   = (atr == 0x02);
			akfile->isDir    = (atr == 0x10);
		
			//kprintf("ART_%d = %d\n", akfile->ID, atr);
			//kprintf("Size_%d = %d\n", akfile->ID, size);
			/*
				   0  1  2  3  4  5  6  7   8  9  A  B  C  D  E  F
			00002600  e5 55 4d 4d 59 20 20 20  20 20 20 20 00 64 5d 62  |.UMMY       .d]b|
			00002610  7c 3d 7c 3d 00 00 5d 62  7c 3d 00 00 00 00 00 00  ||=|=..]b|=......|
			00002620  41 4d 00 79 00 4f 00 53  00 2e 00 0f 00 55 73 00  |AM.y.O.S.....Us.|
			00002630  68 00 00 00 ff ff ff ff  ff ff 00 00 ff ff ff ff  |h...............|
			00002640  4d 59 4f 53 20 20 20 20  53 48 20 20 00 00 cf 75  |MYOS    SH  ...u|
			00002650  7c 3d 7c 3d 00 00 cf 75  7c 3d 03 00 d5 27 00 00  ||=|=...u|=...'..| 54567
			00002660  41 54 00 45 00 53 00 54  00 31 00 0f 00 cb 2e 00  |AT.E.S.T.1......|
			00002670  74 00 78 00 74 00 00 00  ff ff 00 00 ff ff ff ff  |t.x.t...........|
			*/

		
			entry = akfile->clusterID;
			c = 0;
			while(1)
			{
				entry = fat1[entry+2].content;
				if (entry == -1)
					break;
				c++;
			}
			akfile->size = c+1;
		}
		
	}
	
	return root;
}



int readFile( file *f , size_t size, char *des)
{
	int i, entry;
	entry = f->clusterID;
	for(i=0; (0x200*i)<=size; i++)
	{
		
		kmemset((void*) sysbuffer, 0x0, 0x2400);
		readSector( 0x22 + entry - 1 );
		kmemcpy( (void *) (des+0x200*i), (void *) sysbuffer, 512 );
		//kprintf("Read Sec %d!", 0x22 + entry - 1);
		entry = fat1[entry+2].content;
		if (entry == -1)
			break;
		
	}
	return 0;
}
int writeFile(file *f, char *content) //rewrite an existing file
{
	char buf[0x200];
	int sizeofcontent;
	for(sizeofcontent=0;content[sizeofcontent]!='\0'; sizeofcontent++);

	int clusters = sizeofcontent/0x200 + ( sizeofcontent%0x200 != 0 ? 1 : 0 );
	int finishClusters = clusters;
	int lastCluster = 0;
	int i, j, lj = 0;
	j = 0;
	
	int entry = f->clusterID;
	do
	{
		
		lj += 0x200;
		for( ; j < lj ; j++ )
			buf[j-(lj-0x200)] = content[j];
		
		kmemset((void*) buf, sysbuffer, 0x200);
		writeSector( 0x23 + entry );
		entry = fat1[entry+2].content;

		if( finishClusters == 1 ) fat1[entry+2].lastCluster = 1;
		
		finishClusters--;
		
		if (entry == -1)
			break;
	} while(fat1[entry+2].lastCluster == 0);
	
	for(i=0;i<FATSIZE;i++)
	{
		if ( fat1[i].free == 1 && fat1[i].defect == 0 )
		{
			if ( finishClusters == 0 )
				break;
		
			fat1[i].free = 0;
			
			if ( finishClusters == 1 )
				fat1[i].lastCluster = 1;
			else
				fat1[i].lastCluster = 0;
			
			finishClusters--;

			if (lastCluster == 0)
				continue;
			fat1[i].content = lastCluster-2;
			lastCluster = i;

			lj += 0x200;
			for( ; j < lj ; j++ )
				buf[j-(lj-0x200)] = content[j];
			
			kmemset((void*) buf, sysbuffer, 0x200);
			writeSector( 0x23 + i );
			
		}
	}
	return 0;
}

file *getFileById( int id, directory *dir)
{
	id += 2;
	file *akfile = dir->first;
	if (akfile == NULL)
	{
		//Directory Empty!
		return NULL;
	}
	int i = 0;
	while( akfile->next != NULL && id != i)
	{
		if( (int) akfile->next == -1 ) return NULL;
		i++;
		akfile = akfile->next;
	}

	return akfile;
}





void runFile(file *f, size_t size)
{
	/*//Make pagedir
	pageDir_t page = vmmCreateContext();
	void *paddr = pmm_malloc(PAGE_SIZE);
		
	kmemset((void*) page, 0, PAGE_SIZE);
	
	vmmMapPage( page, (void*) 0xfffff000, paddr, USER_PRV ); //0xfffff000
	vmmActivateContext( page );
	
	//end*/

	kprintf("Running app %s\n", f->name);
	char app[size];
	kmemset((void*) &app[0], 0x0, size);
	
	readFile( f, size, &app[0] );

	

	void* load_addr = (void*) 0x400000;

	kmemcpy(load_addr, (void*) &app[0], size);
	//initTask(load_addr, 10);
	
	char *appP = &app[0];
	initELF( (void*) appP, 20, size, "");
}
void USrunFile( file *name, int *pid )
{
	runFile( name, name->size*0x200 );
	kprintf("Fin");
	return;
}
void USlistDir( directory *root, directory **result )
{
	*result = listDir( root );
}
void USreadFile( file *f, char **result, size_t size )
{
	readFile( f, size, *result );
}


