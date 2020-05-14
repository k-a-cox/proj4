#!/usr/bin/env bash

#SBATCH --ntasks=8
#SBATCH --nodes=8
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/pthread_8_thread