#find .//. ! -name . -print | grep -c //
ls -a | wc -l
#ls |wc -l for non hidden files
#ls "$1" |wc -l for command line args