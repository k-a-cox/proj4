#!/bin/bash

#SBATCH --ntasks=16
#SBATCH --nodes=16
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/pthread_16_thread