#!/bin/bash

set -e

echo "Запуск тестов (де)кодера ASCII85..."

TEST_INPUT="Hello, World!"
ENCODED_OUTPUT=$(echo -n "$TEST_INPUT" | ./ascii85 -e)
PYTHON_ENCODED=$(python3 -c "import sys,base64; sys.stdout.write(base64.a85encode(b'$TEST_INPUT').decode())")
if [ "$ENCODED_OUTPUT" != "$PYTHON_ENCODED" ]; then
    echo "Test 1 (Encode Hello, World!) Failed!"
    echo "Ожидалось: $PYTHON_ENCODED"
    echo "Получено:  $ENCODED_OUTPUT"
    exit 1
else
    echo "Test 1 passed."
fi

DECODED_OUTPUT=$(echo -n "$ENCODED_OUTPUT" | ./ascii85 -d)
if [ "$DECODED_OUTPUT" != "$TEST_INPUT" ]; then
    echo "Test 2 (Decode encoded Hello, World!) Failed!"
    echo "Ожидалось: $TEST_INPUT"
    echo "Получено:  $DECODED_OUTPUT"
    exit 1
else
    echo "Test 2 passed."
fi

RANDOM_BIN="random.bin"
RANDOM_ENC="random.enc"
RANDOM_DEC="random.dec"
python3 -c "import os; os.write(1, os.urandom(128))" > "$RANDOM_BIN"
./ascii85 -e < "$RANDOM_BIN" > "$RANDOM_ENC"
./ascii85 -d < "$RANDOM_ENC" > "$RANDOM_DEC"
if ! cmp -s "$RANDOM_BIN" "$RANDOM_DEC"; then
    echo "Test 3 (Random data roundtrip) Failed!"
    exit 1
else
    echo "Test 3 passed."
fi

if ./ascii85 -d <<< "!!~!!"; then
    echo "Test 4 (Invalid input test) Failed! Декодер завершился с кодом 0 на некорректном входе."
    exit 1
else
    echo "Test 4 passed (invalid input detected)."
fi

echo "Все тесты успешно пройдены."
rm -f "$RANDOM_BIN" "$RANDOM_ENC" "$RANDOM_DEC"
