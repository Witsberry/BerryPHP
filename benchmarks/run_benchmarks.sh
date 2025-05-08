#!/bin/bash
# BerryPHP/benchmarks/run_benchmarks.sh
# Runs performance benchmarks.
# Author: Rikaz Zarook, Witsberry

echo "Running benchmarks..."

../run.sh ../berryphp/tests/composer.php &
SERVER_PID=$!
sleep 2
wrk -t4 -c100 -d10s http://localhost:3000 > results_http1.txt

../run.sh ../berryphp/tests/view.php &
SERVER_PID=$!
sleep 2
wrk -t4 -c100 -d10s http://localhost:3000 > results_view.txt

kill $SERVER_PID

echo "Results in results_http1.txt and results_view.txt."