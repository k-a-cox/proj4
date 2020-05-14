#!/bin/bash

#SBATCH --ntasks=4
#SBATCH --nodes=4
SCRIPTPATH=$(dirname "$SCRIPT")
$SCRIPTPATH/pthread_4_thread