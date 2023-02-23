Help()
{
   # Display Help
   echo "Build script"
   echo
   echo "Syntax: test.sh [-t]"
   echo "options:"
   echo "t  Builds test project in addition to quadtree."
   echo
}

buildTests=OFF

while getopts ":ht" option; do
    case $option in
        h) # Help
            Help
            exit;;
        t) # Build tests
            buildTests=ON
    esac
done

rm -rf build
mkdir -p build
cd build
cmake .. -DTP_QUADTREE_BUILD_TESTS=$buildTests
Make