cd ../../bin
echo "Starting subscribers..."
for((i=0;i<10;i++))
do
    ./zmq-sub-sync &
done
echo "Starting publisher..."
./zmq-pub-sync &