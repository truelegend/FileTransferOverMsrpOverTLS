#!/bin/bash

config_file=/root/AutoTestEnv/UCG_COMMON/sipp_project.cfg
SIPP=/root/AutoTestEnv/UCG_COMMON/SIPp/sipp

source $config_file

REMOTE_ADDR=${REMOTE_PUBLIC_IP}:${REMOTE_PUBLIC_PORT}

if [ -z "$1" -o -z "$3" ]; then
    echo "Usage : $0 <MaxNum of Calls> <Calls/sec> <uac scenario name>"
    exit
fi

I=$1
J=$2
S=$3

#${SIPP} -sf $S -nd -m $I -aa -l 4000000 -r $J -t ${TRANSMODE} -i ${SIMULATOR_PUBLIC_IP} -p ${SIMULATOR_PUBLIC_PORT} -mi ${SIMULATOR_PUBLIC_RTP_IP} -mp ${SIMULATOR_PUBLIC_RTP_PORT} ${REMOTE_PUBLIC_IP}:${REMOTE_PUBLIC_PORT} -cid_str '%u-%p@%s' -3pcc 127.0.0.1:${TPCC_INTER_PORT} -trace_msg -trace_logs  -max_socket 65535 -trace_msg -trace_err -trace_logs

echo "${SIPP} -sf $S -inf /tmp/external_sipp.csv -nd -m $I -aa -l 4000000 -r $J -t ${TRANSMODE} -i ${SIMULATOR_PUBLIC_IP} -p ${SIMULATOR_PUBLIC_PORT} -mi ${SIMULATOR_PUBLIC_RTP_IP} -mp ${SIMULATOR_PUBLIC_RTP_PORT} ${REMOTE_PUBLIC_IP}:${REMOTE_PUBLIC_PORT}  -cid_str '%u-%p@%s' -max_socket 65535"


${SIPP} -sf $S -inf /tmp/external_sipp.csv -nd -m $I -aa -l 4000000 -r $J -t ${TRANSMODE} -i ${SIMULATOR_PUBLIC_IP} -p ${SIMULATOR_PUBLIC_PORT} -mi ${SIMULATOR_PUBLIC_RTP_IP} -mp ${SIMULATOR_PUBLIC_RTP_PORT} ${REMOTE_PUBLIC_IP}:${REMOTE_PUBLIC_PORT}  -cid_str '%u-%p@%s' -max_socket 65535
