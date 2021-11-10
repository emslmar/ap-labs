#include <stdio.h>
#include <sys/inotify.h>
#include <ftw.h>
#include <unistd.h>
#include "logger.h"
struct inotify_event *iNotifyEvent;

//Emiliano Sloth
int main(int argc, char** argv){
	int iNotifyAPI;
	int scan;
    int flag = 1;
    char buffer[1024 * (sizeof(struct inotify_event) + 16)];
	int dirWatched;
	char *watchSize;

	if(argc == 2) {
		iNotifyAPI = inotify_init();
		if(iNotifyAPI == -1)
			errorf("Couldn't create the INotifyAPI");
		dirWatched = inotify_add_watch(iNotifyAPI, argv[1], IN_ALL_EVENTS);
		if(dirWatched == -1)
			errorf("Problem with directory watch creation");
        infof("Starting File/Directory Monitor on %s", argv[1]);
		while(flag == 1) {
			scan = read(iNotifyAPI, buffer, 1024*(sizeof(struct inotify_event)+16));
			for (watchSize=buffer; watchSize<buffer+scan;){
				iNotifyEvent = (struct inotify_event*) watchSize;
                if (iNotifyEvent->mask & IN_CREATE){
					if(iNotifyEvent->mask & IN_ISDIR){
                    	printf("[Directory - ");
                	}else{
                    	printf("[File - ");
                	}
		            infof("Create] - %s", iNotifyEvent->name);
				}
	            if (iNotifyEvent->mask & IN_DELETE){
					if(iNotifyEvent->mask & IN_ISDIR){
                    	printf("[Directory - ");
                	}else{
                    	printf("[File - ");
                	}
		            infof("Removal] - %s", iNotifyEvent->name);
				}
	            if (iNotifyEvent->mask & IN_MOVE){
					if(iNotifyEvent->mask & IN_ISDIR){
                    	printf("[Directory - ");
                	}else{
                    	printf("[File - ");
                	}
		            infof("Rename] - %s", iNotifyEvent->name);
				}
				watchSize += sizeof(struct inotify_event)+iNotifyEvent->len;
			}
		}
		inotify_rm_watch(iNotifyAPI, dirWatched);
		close(iNotifyAPI);
	}
	else
		warnf("not enough parameter or too many parameters");
    return 0;
}