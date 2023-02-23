Help()
{
   # Display Help
   echo "Test script. Must have ran build.sh -t beforehand."
   echo
   echo "Syntax: test.sh"
   echo
}

while getopts ":h" option; do
    case $option in
        h) # Help
            Help
            exit;;
    esac
done

./build/qttest