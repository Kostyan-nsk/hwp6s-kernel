#!/bin/sh

#eco
sed -i '25  s/3C,/50,/' *_ipps_para.h # 208
sed -i '112 s/3C,/50,/' *_ipps_para.h # 416
sed -i '113 s/14,/1E,/' *_ipps_para.h # 416
sed -i '199 s/46,/50,/' *_ipps_para.h # 624
sed -i '286 s/50,/55,/' *_ipps_para.h # 798
sed -i '287 s/28,/2D,/' *_ipps_para.h # 798
sed -i '373 s/55,/5A,/' *_ipps_para.h # 1196

sed -i '460 s/5A,/64,/' *_ipps_para.h # 1596
sed -i '461 s/3C,/46,/' *_ipps_para.h # 1596
sed -i '462 s/01,/00,/' *_ipps_para.h # 1596
sed -i '465 s/03,/01,/' *_ipps_para.h # 1596

sed -i '547 s/5F,/FF,/' *_ipps_para.h # 1795
sed -i '548 s/46,/FF,/' *_ipps_para.h # 1795
sed -i '549 s/01,/00,/' *_ipps_para.h # 1795
#sed -i '550 s/01,/00,/' *_ipps_para.h # 1795
sed -i '552 s/02,/00,/' *_ipps_para.h # 1795

sed -i '634 s/64,/FF,/' *_ipps_para.h # 1996
sed -i '635 s/50,/FF,/' *_ipps_para.h # 1996
sed -i '637 s/01,/02,/' *_ipps_para.h # 1996
sed -i '639 s/02,/00,/' *_ipps_para.h # 1996

#norm
sed -i '379 s/55,/5A,/' *_ipps_para.h # 1196
sed -i '466 s/5A,/5F,/' *_ipps_para.h # 1596
sed -i '553 s/5F,/64,/' *_ipps_para.h # 1795
sed -i '555 s/01,/00,/' *_ipps_para.h # 1795
sed -i '640 s/64,/FF,/' *_ipps_para.h # 1996
sed -i '641 s/50,/FF,/' *_ipps_para.h # 1996
#sed -i '643 s/01,/00,/' *_ipps_para.h # 1996
sed -i '645 s/02,/00,/' *_ipps_para.h # 1996

#perf
sed -i '129 s/03,/04,/' *_ipps_para.h # 416
sed -i '211 s/46,/3C,/' *_ipps_para.h # 624
sed -i '212 s/1E,/14,/' *_ipps_para.h # 624
#sed -i '298 s/50,/46,/' *_ipps_para.h # 798
sed -i '299 s/28,/1E,/' *_ipps_para.h # 798
sed -i '303 s/03,/04,/' *_ipps_para.h # 798
sed -i '385 s/55,/50,/' *_ipps_para.h # 1196
sed -i '386 s/32,/28,/' *_ipps_para.h # 1196
sed -i '390 s/03,/04,/' *_ipps_para.h # 1196
sed -i '472 s/5A,/55,/' *_ipps_para.h # 1596
sed -i '473 s/3C,/32,/' *_ipps_para.h # 1596
sed -i '477 s/03,/04,/' *_ipps_para.h # 1596
sed -i '559 s/5F,/5A,/' *_ipps_para.h # 1795
sed -i '560 s/46,/3C,/' *_ipps_para.h # 1795
sed -i '564 s/02,/04,/' *_ipps_para.h # 1795
sed -i '647 s/50,/46,/' *_ipps_para.h # 1996
sed -i '651 s/02,/04,/' *_ipps_para.h # 1996