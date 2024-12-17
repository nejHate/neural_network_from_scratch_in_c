echo "without optimalizating, with runtime checking"
gcc ./main.c -o main -g -Werror -Wextra -Wno-everything -pthread -lm -Wformat-overflow=2 -fsanitize=address -fno-omit-frame-pointer
time ./main

echo ""
echo ""
echo "with -O3, with runtime checking"

gcc ./main.c -o main -Werror -Wextra -Wno-everything -pthread -lm -Wformat-overflow=2 -fsanitize=address -fno-omit-frame-pointer -O3
time ./main

echo ""
echo ""
echo "with coverage test"

gcc ./main.c -o main -ftest-coverage -fprofile-arcs
time ./main

echo ""
echo ""
echo "without optimalizing"

gcc ./main.c -o main
time ./main

echo ""
echo ""
echo "with -03"

gcc ./main.c -o main -O3 -fwhole-program -funroll-all-loops
time ./main

echo ""
echo ""
echo "with -03 -fwhole-program -funroll-all-loops"

gcc ./main.c -o main -O3 -fwhole-program -funroll-all-loops
time ./main

echo ""
echo ""
echo "with -03 -fwhole-program -funroll-all-loops -mavx2"

gcc ./main.c -o main -O3 -fwhole-program -funroll-all-loops -mavx2
time ./main

echo ""
echo ""
echo "with -0fast -fwhole-program -funroll-all-loops -mavx2"

gcc ./main.c -o main -Ofast -fwhole-program -funroll-all-loops -mavx2
time ./main