#!/bin/sh
srcdir=${srcdir:-.}
oIFS="$IFS"
IFS='
'
secno=0
testno=0
comment=0
ecode=0
test -f ${srcdir}/cql2pqfsample || exit 1
test -d cql2pqf || mkdir cql2pqf
while read f; do
	if echo $f | grep '^#' >/dev/null; then
		comment=1
	else
		if test "$comment" = "1"; then
			secno=`expr $secno + 1`
			testno=0
		fi
		comment=0
		testno=`expr $testno + 1`
		OUT1=${srcdir}/cql2pqf/$secno.$testno.out
		ERR1=${srcdir}/cql2pqf/$secno.$testno.err
		OUT2=cql2pqf/$secno.$testno.out.tmp
		ERR2=cql2pqf/$secno.$testno.err.tmp
		DIFF=cql2pqf/$secno.$testno.diff
		../util/cql2pqf -v ${srcdir}/../etc/pqf.properties "$f" >$OUT2 2>$ERR2
		if test -f $OUT1 -a -f $ERR1; then
			if diff $OUT1 $OUT2 >$DIFF; then
				rm $DIFF
				rm $OUT2
			else
				echo "diff out $secno $testno $f"
				cat $DIFF
				ecode=1	
			fi
			if diff $ERR1 $ERR2 >$DIFF; then
				rm $DIFF
				rm $ERR2
			else
				echo "diff err $secno $testno $f"
				cat $DIFF
				ecode=1
			fi
		else
			echo "making test $secno $testno $f"
			mv $OUT2 $OUT1
			mv $ERR2 $ERR1
			ecode=1
		fi	
	fi		
done < cql2pqfsample
IFS="$oIFS"
exit $ecode
