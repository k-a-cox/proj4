#!/bin/bash

#SBATCH --ntasks=1
#SBATCH --nodes=1
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/pthread_1_thread