#!/bin/sh

#echo "Total arguments passed are: $#"

#echo "Arguments are: $*"

#$0 displays the name of the sh file, $1 displays the first argument, etc.
#echo "First argument: $1"
#shift 2

#after shift 2, the value will be displayed in $1 because the pointer is moved (like an offset?)
#echo "First argument after shift 2: $1"

#here obtain the frequency, which should be the first argument
n=$1

#***Code for extra credit starts here***
#get the three files
shift
#name=$1

echo "Number of files: $#"
#need the -e so that echo reads the escape characters properly and knows it is a special syntax
echo -e "Processing: $@\n"

# the symbol $@ will go through the list of arguments based on where the pointer is (if used shift)
# in this case, I shifted first to ignore the first argument (the number of words to display), and only worry about the next arguments (which should be the filenames, how ever many the user wants to input
for i in "$@"
do
#print the name of the file
echo "======$(wc $i) ======"
#print the most common words for the file
echo -e "$(cat $i | tr -cd [a-zA-Z0-9"\n"[:space:]] | tr -s " " "\n" | tr [:upper:] [:lower:] | sort | uniq -c | sort -n -r | head -$n)\n"
shift
done
