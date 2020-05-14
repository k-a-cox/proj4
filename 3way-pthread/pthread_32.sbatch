#!/bin/bash

#SBATCH --ntasks=32
#SBATCH --nodes=16
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/pthread_32_thread