#!/bin/bash

DATA_FILE="/tmp/sample_data.txt"

RESULT_FILE="/tmp/report.txt"

rm -f "$DATA_FILE" "$RESULT_FILE"

download_data() {

    echo "Step 1: Collecting data..."

    sleep 3

    echo -e "value1\nvalue2\nvalue3" > "$DATA_FILE"

    echo "Step 1 completed: data saved in $DATA_FILE"

}

validate_data() {

    echo "Step 2: Validating data..."

    if [ ! -f "$DATA_FILE" ]; then

        echo "ERROR: data is not available yet."

        return 1

    fi

    if [ ! -s "$DATA_FILE" ]; then

        echo "ERROR: data file is empty."

        return 1

    fi

    echo "Data is valid."

    return 0

}

generate_report() {

    echo "Step 3: Generating report..."

    wc -l "$DATA_FILE" > "$RESULT_FILE"

    echo "Report created in $RESULT_FILE"

}

download_data &

PID_DOWNLOAD=$!

echo "Waiting for download process to finish..."

wait $PID_DOWNLOAD

validate_data

STATUS_VALID=$?

if [ $STATUS_VALID -eq 0 ]; then

    generate_report

    echo "Workflow completed successfully."

    echo "Report content:"

    cat "$RESULT_FILE"

else

    echo "Workflow failed because validation did not pass."

fi