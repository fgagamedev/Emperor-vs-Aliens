class direntry
{
public:
  char filename[MAXPATHLEN];
  bool folder;
  bool statfailed;
  int extrainfoint;
  direntry(char* filename2);
};

direntry::direntry(char* filename2)
{
  sprintf(filename,"%s",filename2);
  folder = 0;
  extrainfoint = 0;
}

const int maxdirentries = 500;
direntry *direntries[maxdirentries];
int n_direntries = 0;

void deletedirentry(int number)
{
  delete direntries[number];
  direntries[number] = NULL;
  if (number < n_direntries-1)
  {
    for(int a = number; a < n_direntries; a++)
    {
      direntries[a] = direntries[a+1];
    }
  }
  direntries[n_direntries-1] = NULL;
  n_direntries = n_direntries-1;
}

#ifdef WII
void dirlist(char* directory)
{
  while(n_direntries) deletedirentry(0);
  
  char filepath[MAXPATHLEN];
  struct stat st;
  struct dirent* dirEntry = NULL;
  DIR* dirStruct;

  if ((dirStruct = opendir(directory)))
  {
    if(dirStruct->dirData != NULL)
    {
      while(  (dirEntry = readdir(dirStruct)) != NULL ) 
      {
        direntries[n_direntries] = new direntry(dirEntry->d_name);
        
        if (strcmp (directory, "/") == 0)
        {
          sprintf (filepath, "%s%s", directory, dirEntry->d_name);
        }
        else
        {
          sprintf (filepath, "%s/%s", directory, dirEntry->d_name);
        }
        if(stat(filepath, &st) == 0) 
        {
          direntries[n_direntries]->statfailed = 0;
          if(st.st_mode & S_IFDIR)
          {
            direntries[n_direntries]->folder = 1;
          }
        }
        else
        {
          direntries[n_direntries]->statfailed = 1;
        }
        n_direntries++;
      }
    }
  }
}
#endif


#ifdef WIN
void dirlist(char* directory)
{
  while(n_direntries) deletedirentry(0);
  
  char filename[MAX_PATH];
  WIN32_FIND_DATAA findFileData;
  HANDLE hFind = FindFirstFileA(directory, &findFileData);
  if(hFind != INVALID_HANDLE_VALUE)
  {
    bool endoflist = 0;
    while(endoflist == 0)
    {
      sprintf(filename, "%s", findFileData.cFileName);

      direntries[n_direntries] = new direntry((char*)filename);
      n_direntries++;
      
      if(findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
      {
        direntries[n_direntries]->folder = 1;
      }
      
      if(FindNextFileA(hFind, &findFileData) == 0)
      {
        endoflist = 1;
      }
    }
    //CloseHandle(hFind); //not compiled under win yet so leaving commented...
  }
}
#endif




























/*#ifdef WIN

const int maxdirentrys = 500;
class direntry{
	public:
		char filename[256];
		bool folder;
		int extrainfoint;
		direntry(char* filename2);};
direntry::direntry(char* filename2){
	sprintf(filename,"%s",filename2);
	folder=0;
	extrainfoint=0;}
direntry *direntrys[maxdirentrys];
int n_direntrys = 0;

void deletedirentry(int number){
	delete direntrys[number];
	direntrys[number]=NULL;
	if(number<n_direntrys-1)for(int a=number;a<n_direntrys;a++)direntrys[a]=direntrys[a+1];
	direntrys[n_direntrys-1]=NULL;
	n_direntrys=n_direntrys-1;
}

void dirlist(char* directory){
	while(n_direntrys)deletedirentry(0);
	#ifdef WII
	char filename[MAXPATHLEN];
	struct stat st;
	DIR_ITER* dir;
	dir = diropen (directory);
	if(dir != NULL){
		while(dirnext(dir,filename,&st)==0){
	#endif
	#ifdef WIN
	char filename[MAX_PATH];
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(directory,&findFileData);
	if(hFind != INVALID_HANDLE_VALUE){
		bool endoflist=0;
		while(endoflist==0){
			sprintf(filename,"%s",findFileData.cFileName);
	#endif
			direntrys[n_direntrys] = new direntry((char*)filename);
			#ifdef WII
			if(st.st_mode & S_IFDIR)direntrys[n_direntrys]->folder=1;
			#endif
			#ifdef WIN
			if(findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)direntrys[n_direntrys]->folder=1;
			#endif
			n_direntrys++;
			#ifdef WIN
				if(FindNextFileA(hFind,&findFileData)==0)endoflist=1;
			#endif
		}
	}
}

#endif*/

//class info for each file or filder
/*class direntry{
	public:
		char filename[256];
		bool folder;
};

//class info for file and folder list
class dirlist{
	public:
		int listsize;
		direntry** entries;
};

//get the number of files and folders in a directory
int getdirsize(char* directory){
	int dirsize=0;
	#ifdef WII
	char filename[MAXPATHLEN];
	struct stat st;
	DIR_ITER* dir;
	dir = diropen (directory);
	if(dir != NULL){
		while(dirnext(dir,filename,&st)==0){
	#endif
	#ifdef WIN
	char filename[MAX_PATH];
	WIN32_FIND_DATAA findFileData;
	HANDLE hFind = FindFirstFileA(directory,&findFileData);
	if(hFind != INVALID_HANDLE_VALUE){
		bool endoflist=0;
		while(endoflist==0){
			sprintf(filename,"%s",findFileData.cFileName);
	#endif
			dirsize+=1;
			#ifdef WIN
			if(FindNextFileA(hFind,&findFileData)==0)endoflist=1;
			#endif
		}
	}
	return dirsize;
}

//create a list of files or folders in a directory
dirlist* makedirlist(char* directory){
	//get the list size and give up if its zero
	int dirsize=getdirsize(directory);
	//int dirsize=3;
	if(dirsize>0){
		//make the list
		dirlist* newdirlist = new dirlist;;
		newdirlist->listsize = dirsize;
		newdirlist->entries = new direntry *[dirsize];
		//this is needed to keep track of how far into the list it is
		int entrynum=0;
		//keep checking files untill you read the end of the list
		//while(entrynum<dirsize){
			//sprintf(newdirlist->entries[entrynum]->filename,"%s","hey");
			//newdirlist->entries[entrynum]->folder=1;
			//entrynum+=1;
		//}
		#ifdef WII
		char filename[MAXPATHLEN];
		struct stat st;
		DIR_ITER* dir;
		dir = diropen (directory);
		if(dir != NULL){
			while(dirnext(dir,filename,&st)==0){
		#endif
		#ifdef WIN
		char filename[MAX_PATH];
		WIN32_FIND_DATAA findFileData;
		HANDLE hFind = FindFirstFileA(directory,&findFileData);
		if(hFind != INVALID_HANDLE_VALUE){
			bool endoflist=0;
			while(endoflist==0 && entrynum<dirsize){
				sprintf(filename,"%s",findFileData.cFileName);
		#endif
				//get the file or folder name
				sprintf(newdirlist->entries[entrynum]->filename,"%s",filename);
				//is it a folder or a file
				#ifdef WII
				if(st.st_mode & S_IFDIR)
					newdirlist->entries[entrynum]->folder=1;
				#endif
				#ifdef WIN
				if(findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
					newdirlist->entries[entrynum]->folder=1;
				#endif
				//next entry
				entrynum++;
				//check if done
				#ifdef WIN
					if(FindNextFileA(hFind,&findFileData)==0)endoflist=1;
				#endif
			}
		}
		//return the new list
		return newdirlist;
	}else{
		return NULL;
	}
}*/
