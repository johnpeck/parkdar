v 20110115 2
C 33200 34500 0 0 0 title-bordered-B.sym
T 44900 35600 9 20 1 0 0 0 1
Sonar
T 43500 34800 9 10 1 0 0 0 1
2
T 45100 34800 9 10 1 0 0 0 1
3
T 43800 35100 9 10 1 0 0 0 1
sonar.sch
T 47000 35100 9 10 1 0 0 0 1
A
T 47000 34800 9 10 1 0 0 0 1
John Peck
C 43500 40100 1 0 0 lv_maxsonar_ez0.sym
{
T 45300 40100 5 10 1 1 0 0 1
value=LV-MaxSonar-EZ0
T 45300 40300 5 10 1 1 0 0 1
refdes=MOD?
}
C 42100 39700 1 0 0 6pin_mta100_header_in_ra.sym
{
T 42100 39700 5 10 1 1 0 0 1
value=mta100
T 42100 42300 5 10 1 1 0 0 1
refdes=J?
}
C 40000 40600 1 0 0 6pin_mta100_white_recept.sym
{
T 40400 41300 5 10 1 1 0 0 1
refdes=MECH?
T 40400 41100 5 10 1 1 0 0 1
value=mta100
T 40400 40900 5 10 1 1 0 0 1
description=6-pin, white
T 40400 40700 5 10 1 1 0 0 1
comment=24-guage stranded
}
C 36600 42100 1 0 0 2pin_mta100_white_recept.sym
{
T 37000 42800 5 10 1 1 0 0 1
refdes=MECH?
T 37000 42600 5 10 1 1 0 0 1
value=mta100
T 37000 42400 5 10 1 1 0 0 1
description=2-pin, white
T 37000 42200 5 10 1 1 0 0 1
comment=24-guage stranded
}
C 36600 38500 1 0 0 2pin_mta100_white_recept.sym
{
T 37000 39200 5 10 1 1 0 0 1
refdes=MECH?
T 37000 39000 5 10 1 1 0 0 1
value=mta100
T 37000 38800 5 10 1 1 0 0 1
description=2-pin, white
T 37000 38600 5 10 1 1 0 0 1
comment=24-guage stranded
}
N 42900 42000 44300 42000 4
N 42900 41600 44300 41600 4
N 42900 41200 44300 41200 4
N 42900 40800 44300 40800 4
N 42900 40400 43600 40400 4
N 43600 40400 43600 43200 4
N 43600 43200 45200 43200 4
N 45200 43200 45200 43000 4
N 42900 40000 45200 40000 4
N 45200 40000 45200 40200 4
T 39100 40300 9 10 1 0 0 0 1
3: White to ADC pin 1
T 39100 40100 9 10 1 0 0 0 1
6: Red to +3.3 pin 1
T 39100 39900 9 10 1 0 0 0 1
7: Black to Y-split, then both pin 2
T 36600 37800 9 10 1 0 0 0 3
To power on pencils board
1: +3.3
2: Ground
T 36600 41400 9 10 1 0 0 0 3
To butterfly voltage reader
1: Vin
2: Ground
