#!/usr/bin/env bash

echo "Installing Sparkle keys to keychain..."

if [ -z "${SPARKLE_KEY}" ]; then
    echo "SPARKLE_KEY must be defined" >&2
    exit 1
fi


export KEYCHAIN=build.keychain


security create-keychain -p uavos $KEYCHAIN
security default-keychain -s $KEYCHAIN
security unlock-keychain -p uavos $KEYCHAIN
security add-generic-password -U -a 'ed25519' -D 'private key' -s 'https://sparkle-project.org' -w $SPARKLE_KEY -A $KEYCHAIN; \
security set-keychain-settings $KEYCHAIN; \
security list-keychains -s $KEYCHAIN; \
security set-generic-password-partition-list -k uavos -s 'https://sparkle-project.org' -S apple-tool:,apple:,security:,unsigned: $KEYCHAIN; \

echo "Sparkle keys installed."
