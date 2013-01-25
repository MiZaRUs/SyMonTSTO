#!/usr/bin/perl
$ind = 1;
#------------------------
#$nameurl = "/home/oleg/Rab/Perl/tm/tmgo ";
#$command = $nameurl;
#$command .= $kan;
#$dan = `$command`;
#------------------------
@archiv;
while(ind){
    open (HF, "s_vds1_zz.x.fifo") || die "File not found\n";
    $it = 0;
    $pa = 0;
    while (<HF>){	# читать по одной строке в переммен по умолч $_
#        print  $_;
        @list = split(/:/,$_);
        if($it == 0){
            if($list[1] < 0 ){
                print "SOCKET!!!\n";
            }else{
                $pa = $list[1];
            }
        }else{
            if($list[1] != $archiv[$it]){
                $archiv[$it] = $list[1];
                print $_;
            }
        }

        $it++;
    }
    close(HF);

    print ".\n";
    sleep(5);
}# while
exit;
