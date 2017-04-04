#!/bin/bash


#extract data from cdrom

echo "extracting data from your ascendancy cdrom"
mkdir ascend
cd ascend
../parser /home/lemon/Documents/ascendancy/ASCEND00.COB
../parser /home/lemon/Documents/ascendancy/ASCEND01.COB
../parser /home/lemon/Documents/ascendancy/ASCEND02.COB

#enter data directory
#mv big.ttf ./data
echo "Entering data directory"
cd data

#first convert all musics
echo "Creating musics directory"
mkdir music
echo "Moving musics"
mv *.raw music/
cd music

echo "Converting all of these nasty .raw to .wav"
for i in *.raw ; do
echo $i;
sox -r 22050 -u -b -c 1 $i `basename $i .raw`.wav ; 
rm $i ; 
done

#try if the resample command is also here to enhance sound quality
if [ -e "/usr/bin/resample" ]; then
	echo "enhancing sound quality"
	for i in *.wav ; do
		rm -f tmp.wav
		mv $i tmp.wav
		resample -by 2 tmp.wav $i
	done
	rm -f tmp.wav
fi

#now encode all wav files into ogg
echo "compressing music"
for i in *.wav ; do
	oggenc $i
	rm $i
done

cd ..

#now move and convert all samples

echo "Creating sounds directory"
mkdir sounds
echo "Moving sounds"
mv *.voc sounds/
cd sounds

echo "Converting all of these nasty disguised .raw to .wav"

for i in *.voc ; do
echo $i;
sox -t raw -r 22500 -u -b -c 1 $i `basename $i .voc`.wav ; 
rm $i ; 
done

#try if the resample command is also here to enhance sound quality
if [ -e "/usr/bin/resample" ]; then
	echo "enhancing sound quality"
	for i in *.wav ; do
		rm -f tmp.wav
		mv $i tmp.wav
		resample -by 2 tmp.wav $i
	done
	rm -f tmp.wav
fi


cd ..

#now go on with images 

echo "Creating images directory"
mkdir graphx
echo "Moving images"
mv *.shp graphx
mv *.tmp graphx
mv *.gif graphx
mv game.pal graphx
cp ../../tux.png graphx
cd graphx

echo "Converting .shp into png files"

for i in *shp ; do
../../../imgbuild $i;
done

echo "Converting .tmp into png files"

for i in *tmp ; do
../../../imgbuild $i;
done

echo "Renaming different color images"
cd racering
mv 0_racering.png a0_racering.png
mv 1_racering.png b0_racering.png
mv 2_racering.png c0_racering.png
mv 3_racering.png d0_racering.png
mv 4_racering.png e0_racering.png
mv 5_racering.png f0_racering.png
mv 6_racering.png g0_racering.png
cd ..

echo "Generating some color images"
cd raceflag
for i in *png ; do ../../../../colorcvt $i ; done
cd ..

cd ..

echo "Finished!"
