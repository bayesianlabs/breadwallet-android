
#define BITCOIN_TEST_NO_MAIN = 1;

#include "core.h"
#include "wallet.h"
#include <stdio.h>
#include "breadwallet-core/BRPaymentProtocol.h"
#include "breadwallet-core/BRTransaction.h"
#include "breadwallet-core/BRAddress.h"
#include "breadwallet-core/BRWallet.h"
#include <android/log.h>

//TODO make sure to free() everything

//
// Created by Mihail Gutan on 9/24/15.
//
const int TEST_REQ = 0;

const char req[] = "\x12\x0b\x78\x35\x30\x39\x2b\x73\x68\x61\x32\x35\x36\x1a\xbe\x15\x0a\xfe\x0b\x30\x82\x05\xfa"
        "\x30\x82\x04\xe2\xa0\x03\x02\x01\x02\x02\x10\x09\x0b\x35\xca\x5c\x5b\xf1\xb9\x8b\x3d\x8f\x9f\x4a\x77\x55\xd6\x30"
        "\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x0b\x05\x00\x30\x75\x31\x0b\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55"
        "\x53\x31\x15\x30\x13\x06\x03\x55\x04\x0a\x13\x0c\x44\x69\x67\x69\x43\x65\x72\x74\x20\x49\x6e\x63\x31\x19\x30\x17"
        "\x06\x03\x55\x04\x0b\x13\x10\x77\x77\x77\x2e\x64\x69\x67\x69\x63\x65\x72\x74\x2e\x63\x6f\x6d\x31\x34\x30\x32\x06"
        "\x03\x55\x04\x03\x13\x2b\x44\x69\x67\x69\x43\x65\x72\x74\x20\x53\x48\x41\x32\x20\x45\x78\x74\x65\x6e\x64\x65\x64"
        "\x20\x56\x61\x6c\x69\x64\x61\x74\x69\x6f\x6e\x20\x53\x65\x72\x76\x65\x72\x20\x43\x41\x30\x1e\x17\x0d\x31\x34\x30"
        "\x35\x30\x39\x30\x30\x30\x30\x30\x30\x5a\x17\x0d\x31\x36\x30\x35\x31\x33\x31\x32\x30\x30\x30\x30\x5a\x30\x82\x01"
        "\x05\x31\x1d\x30\x1b\x06\x03\x55\x04\x0f\x0c\x14\x50\x72\x69\x76\x61\x74\x65\x20\x4f\x72\x67\x61\x6e\x69\x7a\x61"
        "\x74\x69\x6f\x6e\x31\x13\x30\x11\x06\x0b\x2b\x06\x01\x04\x01\x82\x37\x3c\x02\x01\x03\x13\x02\x55\x53\x31\x19\x30"
        "\x17\x06\x0b\x2b\x06\x01\x04\x01\x82\x37\x3c\x02\x01\x02\x13\x08\x44\x65\x6c\x61\x77\x61\x72\x65\x31\x10\x30\x0e"
        "\x06\x03\x55\x04\x05\x13\x07\x35\x31\x35\x34\x33\x31\x37\x31\x0f\x30\x0d\x06\x03\x55\x04\x09\x0c\x06\x23\x32\x33"
        "\x30\x30\x38\x31\x17\x30\x15\x06\x03\x55\x04\x09\x13\x0e\x35\x34\x38\x20\x4d\x61\x72\x6b\x65\x74\x20\x53\x74\x2e"
        "\x31\x0e\x30\x0c\x06\x03\x55\x04\x11\x13\x05\x39\x34\x31\x30\x34\x31\x0b\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55"
        "\x53\x31\x13\x30\x11\x06\x03\x55\x04\x08\x13\x0a\x43\x61\x6c\x69\x66\x6f\x72\x6e\x69\x61\x31\x16\x30\x14\x06\x03"
        "\x55\x04\x07\x13\x0d\x53\x61\x6e\x20\x46\x72\x61\x6e\x63\x69\x73\x63\x6f\x31\x17\x30\x15\x06\x03\x55\x04\x0a\x13"
        "\x0e\x43\x6f\x69\x6e\x62\x61\x73\x65\x2c\x20\x49\x6e\x63\x2e\x31\x15\x30\x13\x06\x03\x55\x04\x03\x13\x0c\x63\x6f"
        "\x69\x6e\x62\x61\x73\x65\x2e\x63\x6f\x6d\x30\x82\x01\x22\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x01\x05"
        "\x00\x03\x82\x01\x0f\x00\x30\x82\x01\x0a\x02\x82\x01\x01\x00\xb4\x5e\x3f\xf3\x80\x66\x7a\xa1\x4d\x5a\x12\xfc\x2f"
        "\xc9\x83\xfc\x66\x18\xb5\x54\x99\x93\x3c\x3b\xde\x15\xc0\x1d\x83\x88\x46\xb4\xca\xf9\x84\x8e\x7c\x40\xe5\xfa\x7c"
        "\x67\xef\x9b\x5b\x1e\xfe\x26\xee\x55\x71\xc5\xfa\x2e\xff\x75\x90\x52\x45\x47\x01\xad\x89\x31\x55\x7d\x69\x7b\x13"
        "\x9e\x5d\x19\xab\xb3\xe4\x39\x67\x5f\x31\xdb\x7f\x2e\xf1\xa5\xd9\x7d\xb0\x7c\x1f\x69\x66\x26\x63\x80\xeb\x4f\xcf"
        "\xa8\xe1\x47\x1a\x6e\xcc\x2f\xbe\xbf\x3e\x67\xb3\xea\xa8\x4d\x0f\xbe\x06\x3e\x60\x38\x0d\xcd\xb7\xa2\x02\x03\xd2"
        "\x9a\x94\x05\x9e\xf7\xf2\x0d\x47\x2c\xc2\x57\x83\xab\x2a\x1d\xb6\xa3\x94\xec\xc0\x7b\x40\x24\x97\x41\x00\xbc\xfd"
        "\x47\x0f\x59\xef\x3b\x57\x23\x65\x21\x32\x09\x60\x9f\xad\x22\x99\x94\xb4\x92\x3c\x1d\xf3\xa1\x8c\x41\xe3\xe7\xbc"
        "\x1f\x19\x2b\xa6\xe7\xe5\xc3\x2a\xe1\x55\x10\x7e\x21\x90\x3e\xff\x7b\xce\x9f\xc5\x94\xb4\x9d\x9f\x6a\xe7\x90\x1f"
        "\xa1\x91\xfc\xba\xe8\xa2\xcf\x09\xc3\xbf\xc2\x43\x77\xd7\x17\xb6\x01\x00\x80\xc5\x68\x1a\x7d\xbc\x6e\x1d\x52\x98"
        "\x7b\x7e\xbb\xe9\x5e\x7a\xf4\x20\x2d\xa4\x36\xe6\x7a\x88\x47\x2a\xac\xed\xc9\x02\x03\x01\x00\x01\xa3\x82\x01\xf2"
        "\x30\x82\x01\xee\x30\x1f\x06\x03\x55\x1d\x23\x04\x18\x30\x16\x80\x14\x3d\xd3\x50\xa5\xd6\xa0\xad\xee\xf3\x4a\x60"
        "\x0a\x65\xd3\x21\xd4\xf8\xf8\xd6\x0f\x30\x1d\x06\x03\x55\x1d\x0e\x04\x16\x04\x14\x6d\x33\xb9\x74\x3a\x61\xb7\x49"
        "\x94\x23\xd1\xa8\x9d\x08\x5d\x01\x48\x68\x0b\xba\x30\x29\x06\x03\x55\x1d\x11\x04\x22\x30\x20\x82\x0c\x63\x6f\x69"
        "\x6e\x62\x61\x73\x65\x2e\x63\x6f\x6d\x82\x10\x77\x77\x77\x2e\x63\x6f\x69\x6e\x62\x61\x73\x65\x2e\x63\x6f\x6d\x30"
        "\x0e\x06\x03\x55\x1d\x0f\x01\x01\xff\x04\x04\x03\x02\x05\xa0\x30\x1d\x06\x03\x55\x1d\x25\x04\x16\x30\x14\x06\x08"
        "\x2b\x06\x01\x05\x05\x07\x03\x01\x06\x08\x2b\x06\x01\x05\x05\x07\x03\x02\x30\x75\x06\x03\x55\x1d\x1f\x04\x6e\x30"
        "\x6c\x30\x34\xa0\x32\xa0\x30\x86\x2e\x68\x74\x74\x70\x3a\x2f\x2f\x63\x72\x6c\x33\x2e\x64\x69\x67\x69\x63\x65\x72"
        "\x74\x2e\x63\x6f\x6d\x2f\x73\x68\x61\x32\x2d\x65\x76\x2d\x73\x65\x72\x76\x65\x72\x2d\x67\x31\x2e\x63\x72\x6c\x30"
        "\x34\xa0\x32\xa0\x30\x86\x2e\x68\x74\x74\x70\x3a\x2f\x2f\x63\x72\x6c\x34\x2e\x64\x69\x67\x69\x63\x65\x72\x74\x2e"
        "\x63\x6f\x6d\x2f\x73\x68\x61\x32\x2d\x65\x76\x2d\x73\x65\x72\x76\x65\x72\x2d\x67\x31\x2e\x63\x72\x6c\x30\x42\x06"
        "\x03\x55\x1d\x20\x04\x3b\x30\x39\x30\x37\x06\x09\x60\x86\x48\x01\x86\xfd\x6c\x02\x01\x30\x2a\x30\x28\x06\x08\x2b"
        "\x06\x01\x05\x05\x07\x02\x01\x16\x1c\x68\x74\x74\x70\x73\x3a\x2f\x2f\x77\x77\x77\x2e\x64\x69\x67\x69\x63\x65\x72"
        "\x74\x2e\x63\x6f\x6d\x2f\x43\x50\x53\x30\x81\x88\x06\x08\x2b\x06\x01\x05\x05\x07\x01\x01\x04\x7c\x30\x7a\x30\x24"
        "\x06\x08\x2b\x06\x01\x05\x05\x07\x30\x01\x86\x18\x68\x74\x74\x70\x3a\x2f\x2f\x6f\x63\x73\x70\x2e\x64\x69\x67\x69"
        "\x63\x65\x72\x74\x2e\x63\x6f\x6d\x30\x52\x06\x08\x2b\x06\x01\x05\x05\x07\x30\x02\x86\x46\x68\x74\x74\x70\x3a\x2f"
        "\x2f\x63\x61\x63\x65\x72\x74\x73\x2e\x64\x69\x67\x69\x63\x65\x72\x74\x2e\x63\x6f\x6d\x2f\x44\x69\x67\x69\x43\x65"
        "\x72\x74\x53\x48\x41\x32\x45\x78\x74\x65\x6e\x64\x65\x64\x56\x61\x6c\x69\x64\x61\x74\x69\x6f\x6e\x53\x65\x72\x76"
        "\x65\x72\x43\x41\x2e\x63\x72\x74\x30\x0c\x06\x03\x55\x1d\x13\x01\x01\xff\x04\x02\x30\x00\x30\x0d\x06\x09\x2a\x86"
        "\x48\x86\xf7\x0d\x01\x01\x0b\x05\x00\x03\x82\x01\x01\x00\xaa\xdf\xcf\x94\x05\x0e\xd9\x38\xe3\x11\x4a\x64\x0a\xf3"
        "\xd9\xb0\x42\x76\xda\x00\xf5\x21\x5d\x71\x48\xf9\xf1\x6d\x4c\xac\x0c\x77\xbd\x53\x49\xec\x2f\x47\x29\x9d\x03\xc9"
        "\x00\xf7\x01\x46\x75\x2d\xa7\x28\x29\x29\x0a\xc5\x0a\x77\x99\x2f\x01\x53\x7a\xb2\x68\x93\x92\xce\x0b\xfe\xb7\xef"
        "\xa4\x9f\x4c\x4f\xe4\xe1\xe4\x3c\xa1\xfc\xfb\x16\x26\xce\x55\x4d\xa4\xf6\xe7\xfa\x34\xa5\x97\xe4\x01\xf2\x15\xc4"
        "\x3a\xfd\x0b\xa7\x77\xad\x58\x7e\xb0\xaf\xac\xd7\x1f\x7a\x6a\xf7\x75\x28\x14\xf7\xab\x4c\x20\x2e\xd7\x6d\x33\xde"
        "\xfd\x12\x89\xd5\x41\x80\x3f\xed\x01\xac\x80\xa3\xca\xcf\xda\xae\x29\x27\x9e\x5d\xe1\x4d\x46\x04\x75\xf4\xba\xf2"
        "\x7e\xab\x69\x33\x79\xd3\x91\x20\xe7\x47\x7b\xf3\xec\x71\x96\x64\xc7\xb6\xcb\x5e\x55\x75\x56\xe5\xbb\xdd\xd9\xc9"
        "\xd1\xeb\xc9\xf8\x35\xe9\xda\x5b\x3d\xbb\x72\xfe\x8d\x94\xac\x05\xea\xb3\xc4\x79\x98\x75\x20\xad\xe3\xa1\xd2\x75"
        "\xe1\xe2\xfe\x72\x56\x98\xd2\xf7\xcb\x13\x90\xa9\xd4\x0e\xa6\xcb\xf2\x1a\x73\xbd\xdc\xcd\x1a\xd6\x1a\xa2\x49\xce"
        "\x8e\x28\x85\xa3\x73\x0b\x7d\x53\xbd\x07\x5f\x55\x09\x9d\x29\x60\xf3\xcc\x0a\xba\x09\x30\x82\x04\xb6\x30\x82\x03"
        "\x9e\xa0\x03\x02\x01\x02\x02\x10\x0c\x79\xa9\x44\xb0\x8c\x11\x95\x20\x92\x61\x5f\xe2\x6b\x1d\x83\x30\x0d\x06\x09"
        "\x2a\x86\x48\x86\xf7\x0d\x01\x01\x0b\x05\x00\x30\x6c\x31\x0b\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55\x53\x31\x15"
        "\x30\x13\x06\x03\x55\x04\x0a\x13\x0c\x44\x69\x67\x69\x43\x65\x72\x74\x20\x49\x6e\x63\x31\x19\x30\x17\x06\x03\x55"
        "\x04\x0b\x13\x10\x77\x77\x77\x2e\x64\x69\x67\x69\x63\x65\x72\x74\x2e\x63\x6f\x6d\x31\x2b\x30\x29\x06\x03\x55\x04"
        "\x03\x13\x22\x44\x69\x67\x69\x43\x65\x72\x74\x20\x48\x69\x67\x68\x20\x41\x73\x73\x75\x72\x61\x6e\x63\x65\x20\x45"
        "\x56\x20\x52\x6f\x6f\x74\x20\x43\x41\x30\x1e\x17\x0d\x31\x33\x31\x30\x32\x32\x31\x32\x30\x30\x30\x30\x5a\x17\x0d"
        "\x32\x38\x31\x30\x32\x32\x31\x32\x30\x30\x30\x30\x5a\x30\x75\x31\x0b\x30\x09\x06\x03\x55\x04\x06\x13\x02\x55\x53"
        "\x31\x15\x30\x13\x06\x03\x55\x04\x0a\x13\x0c\x44\x69\x67\x69\x43\x65\x72\x74\x20\x49\x6e\x63\x31\x19\x30\x17\x06"
        "\x03\x55\x04\x0b\x13\x10\x77\x77\x77\x2e\x64\x69\x67\x69\x63\x65\x72\x74\x2e\x63\x6f\x6d\x31\x34\x30\x32\x06\x03"
        "\x55\x04\x03\x13\x2b\x44\x69\x67\x69\x43\x65\x72\x74\x20\x53\x48\x41\x32\x20\x45\x78\x74\x65\x6e\x64\x65\x64\x20"
        "\x56\x61\x6c\x69\x64\x61\x74\x69\x6f\x6e\x20\x53\x65\x72\x76\x65\x72\x20\x43\x41\x30\x82\x01\x22\x30\x0d\x06\x09"
        "\x2a\x86\x48\x86\xf7\x0d\x01\x01\x01\x05\x00\x03\x82\x01\x0f\x00\x30\x82\x01\x0a\x02\x82\x01\x01\x00\xd7\x53\xa4"
        "\x04\x51\xf8\x99\xa6\x16\x48\x4b\x67\x27\xaa\x93\x49\xd0\x39\xed\x0c\xb0\xb0\x00\x87\xf1\x67\x28\x86\x85\x8c\x8e"
        "\x63\xda\xbc\xb1\x40\x38\xe2\xd3\xf5\xec\xa5\x05\x18\xb8\x3d\x3e\xc5\x99\x17\x32\xec\x18\x8c\xfa\xf1\x0c\xa6\x64"
        "\x21\x85\xcb\x07\x10\x34\xb0\x52\x88\x2b\x1f\x68\x9b\xd2\xb1\x8f\x12\xb0\xb3\xd2\xe7\x88\x1f\x1f\xef\x38\x77\x54"
        "\x53\x5f\x80\x79\x3f\x2e\x1a\xaa\xa8\x1e\x4b\x2b\x0d\xab\xb7\x63\xb9\x35\xb7\x7d\x14\xbc\x59\x4b\xdf\x51\x4a\xd2"
        "\xa1\xe2\x0c\xe2\x90\x82\x87\x6a\xae\xea\xd7\x64\xd6\x98\x55\xe8\xfd\xaf\x1a\x50\x6c\x54\xbc\x11\xf2\xfd\x4a\xf2"
        "\x9d\xbb\x7f\x0e\xf4\xd5\xbe\x8e\x16\x89\x12\x55\xd8\xc0\x71\x34\xee\xf6\xdc\x2d\xec\xc4\x87\x25\x86\x8d\xd8\x21"
        "\xe4\xb0\x4d\x0c\x89\xdc\x39\x26\x17\xdd\xf6\xd7\x94\x85\xd8\x04\x21\x70\x9d\x6f\x6f\xff\x5c\xba\x19\xe1\x45\xcb"
        "\x56\x57\x28\x7e\x1c\x0d\x41\x57\xaa\xb7\xb8\x27\xbb\xb1\xe4\xfa\x2a\xef\x21\x23\x75\x1a\xad\x2d\x9b\x86\x35\x8c"
        "\x9c\x77\xb5\x73\xad\xd8\x94\x2d\xe4\xf3\x0c\x9d\xee\xc1\x4e\x62\x7e\x17\xc0\x71\x9e\x2c\xde\xf1\xf9\x10\x28\x19"
        "\x33\x02\x03\x01\x00\x01\xa3\x82\x01\x49\x30\x82\x01\x45\x30\x12\x06\x03\x55\x1d\x13\x01\x01\xff\x04\x08\x30\x06"
        "\x01\x01\xff\x02\x01\x00\x30\x0e\x06\x03\x55\x1d\x0f\x01\x01\xff\x04\x04\x03\x02\x01\x86\x30\x1d\x06\x03\x55\x1d"
        "\x25\x04\x16\x30\x14\x06\x08\x2b\x06\x01\x05\x05\x07\x03\x01\x06\x08\x2b\x06\x01\x05\x05\x07\x03\x02\x30\x34\x06"
        "\x08\x2b\x06\x01\x05\x05\x07\x01\x01\x04\x28\x30\x26\x30\x24\x06\x08\x2b\x06\x01\x05\x05\x07\x30\x01\x86\x18\x68"
        "\x74\x74\x70\x3a\x2f\x2f\x6f\x63\x73\x70\x2e\x64\x69\x67\x69\x63\x65\x72\x74\x2e\x63\x6f\x6d\x30\x4b\x06\x03\x55"
        "\x1d\x1f\x04\x44\x30\x42\x30\x40\xa0\x3e\xa0\x3c\x86\x3a\x68\x74\x74\x70\x3a\x2f\x2f\x63\x72\x6c\x34\x2e\x64\x69"
        "\x67\x69\x63\x65\x72\x74\x2e\x63\x6f\x6d\x2f\x44\x69\x67\x69\x43\x65\x72\x74\x48\x69\x67\x68\x41\x73\x73\x75\x72"
        "\x61\x6e\x63\x65\x45\x56\x52\x6f\x6f\x74\x43\x41\x2e\x63\x72\x6c\x30\x3d\x06\x03\x55\x1d\x20\x04\x36\x30\x34\x30"
        "\x32\x06\x04\x55\x1d\x20\x00\x30\x2a\x30\x28\x06\x08\x2b\x06\x01\x05\x05\x07\x02\x01\x16\x1c\x68\x74\x74\x70\x73"
        "\x3a\x2f\x2f\x77\x77\x77\x2e\x64\x69\x67\x69\x63\x65\x72\x74\x2e\x63\x6f\x6d\x2f\x43\x50\x53\x30\x1d\x06\x03\x55"
        "\x1d\x0e\x04\x16\x04\x14\x3d\xd3\x50\xa5\xd6\xa0\xad\xee\xf3\x4a\x60\x0a\x65\xd3\x21\xd4\xf8\xf8\xd6\x0f\x30\x1f"
        "\x06\x03\x55\x1d\x23\x04\x18\x30\x16\x80\x14\xb1\x3e\xc3\x69\x03\xf8\xbf\x47\x01\xd4\x98\x26\x1a\x08\x02\xef\x63"
        "\x64\x2b\xc3\x30\x0d\x06\x09\x2a\x86\x48\x86\xf7\x0d\x01\x01\x0b\x05\x00\x03\x82\x01\x01\x00\x9d\xb6\xd0\x90\x86"
        "\xe1\x86\x02\xed\xc5\xa0\xf0\x34\x1c\x74\xc1\x8d\x76\xcc\x86\x0a\xa8\xf0\x4a\x8a\x42\xd6\x3f\xc8\xa9\x4d\xad\x7c"
        "\x08\xad\xe6\xb6\x50\xb8\xa2\x1a\x4d\x88\x07\xb1\x29\x21\xdc\xe7\xda\xc6\x3c\x21\xe0\xe3\x11\x49\x70\xac\x7a\x1d"
        "\x01\xa4\xca\x11\x3a\x57\xab\x7d\x57\x2a\x40\x74\xfd\xd3\x1d\x85\x18\x50\xdf\x57\x47\x75\xa1\x7d\x55\x20\x2e\x47"
        "\x37\x50\x72\x8c\x7f\x82\x1b\xd2\x62\x8f\x2d\x03\x5a\xda\xc3\xc8\xa1\xce\x2c\x52\xa2\x00\x63\xeb\x73\xba\x71\xc8"
        "\x49\x27\x23\x97\x64\x85\x9e\x38\x0e\xad\x63\x68\x3c\xba\x52\x81\x58\x79\xa3\x2c\x0c\xdf\xde\x6d\xeb\x31\xf2\xba"
        "\xa0\x7c\x6c\xf1\x2c\xd4\xe1\xbd\x77\x84\x37\x03\xce\x32\xb5\xc8\x9a\x81\x1a\x4a\x92\x4e\x3b\x46\x9a\x85\xfe\x83"
        "\xa2\xf9\x9e\x8c\xa3\xcc\x0d\x5e\xb3\x3d\xcf\x04\x78\x8f\x14\x14\x7b\x32\x9c\xc7\x00\xa6\x5c\xc4\xb5\xa1\x55\x8d"
        "\x5a\x56\x68\xa4\x22\x70\xaa\x3c\x81\x71\xd9\x9d\xa8\x45\x3b\xf4\xe5\xf6\xa2\x51\xdd\xc7\x7b\x62\xe8\x6f\x0c\x74"
        "\xeb\xb8\xda\xf8\xbf\x87\x0d\x79\x50\x91\x90\x9b\x18\x3b\x91\x59\x27\xf1\x35\x28\x13\xab\x26\x7e\xd5\xf7\x7a\x22"
        "\xb4\x01\x12\x1f\x08\x98\xb7\x68\x12\x19\x76\xa9\x14\x7d\x53\x25\xa8\x54\xf0\xc9\xa1\xcb\xb6\xcb\xfb\x89\xb2\xa9"
        "\x6d\x83\x7e\xd7\xbf\x88\xac\x18\xac\xb9\xe0\x9e\x05\x20\xd2\xbc\xe0\x9e\x05\x2a\x31\x50\x61\x79\x6d\x65\x6e\x74"
        "\x20\x72\x65\x71\x75\x65\x73\x74\x20\x66\x6f\x72\x20\x43\x6f\x69\x6e\x62\x61\x73\x65\x20\x6f\x72\x64\x65\x72\x20"
        "\x63\x6f\x64\x65\x3a\x20\x51\x43\x4f\x49\x47\x44\x50\x41\x32\x30\x68\x74\x74\x70\x73\x3a\x2f\x2f\x63\x6f\x69\x6e"
        "\x62\x61\x73\x65\x2e\x63\x6f\x6d\x2f\x72\x70\x2f\x35\x33\x64\x38\x31\x62\x66\x61\x35\x64\x36\x62\x31\x64\x64\x61"
        "\x37\x62\x30\x30\x30\x30\x30\x34\x3a\x20\x33\x36\x32\x64\x32\x39\x31\x39\x32\x31\x37\x36\x32\x31\x33\x39\x32\x35"
        "\x38\x37\x36\x63\x65\x32\x63\x62\x34\x30\x30\x34\x31\x62\x2a\x80\x02\x4d\x81\xca\x72\x21\x38\x13\xb2\x58\x5d\x98"
        "\x00\x5b\x23\x8e\x26\x8a\x00\x9e\xc0\x2d\x04\xdd\x7a\x8a\x98\x48\x32\xb9\x90\xd7\x40\xa9\x69\x09\xd6\x2a\x5d\xf9"
        "\xf8\xf8\x5b\x67\x32\x93\x79\xbb\xa0\xa9\xba\x03\xbc\xa3\xd6\x14\x00\xd4\xe4\x77\x98\x4b\x7e\xdc\xf3\x04\x22\x61"
        "\x71\x84\x23\x73\x6c\x44\x1d\x14\x0e\xe8\x9d\x64\x60\x96\x67\xde\x50\xea\xdb\x4c\xab\xbe\xf4\x78\xd3\xa9\xcb\xd4"
        "\xdf\xda\xb9\xa0\xc2\x81\x83\x90\xd2\x0c\x24\x3a\xd0\x2c\xc2\x7a\xbf\x0b\xbb\x2b\xab\x32\x27\xba\xa8\xe5\xd6\x73"
        "\xf8\x49\x91\x41\x22\x53\xbe\x1e\x69\xdf\xa7\x80\xdc\x06\xb6\xf4\x8e\xdf\xa1\x5d\xe6\xd0\xcc\xec\x22\xd9\xfa\xaf"
        "\x67\xb5\x35\xe8\xb2\x77\x8c\xdf\x61\x84\xda\x2f\x2d\x17\x92\xd3\x4c\x64\x40\x98\x83\x27\x32\x9e\x9c\x5a\xe1\x8c"
        "\x34\xdd\xa1\x6d\xcd\xfb\xf4\x19\xf7\xfd\x27\xbf\x57\x5b\x6f\x9c\x95\xb1\xf0\x90\x02\x16\x40\xaf\x5c\x02\xad\x02"
        "\x7b\x5d\x76\x05\x3a\x58\x40\xbc\x4d\x61\x04\xdd\x87\xef\xc3\x1b\xcc\x3a\x8a\xef\xc3\x10\x02\x35\xbe\x61\xc0\x3a"
        "\x50\x55\x66\x77\x71\x85\xdd\x6f\x93\x2b\xae\xb5\xd5\xe2\xd4\x39\x8d\x01\x14\x0d\x48";

JNIEXPORT jobject Java_com_breadwallet_tools_security_RequestHandler_parsePaymentRequest
        (JNIEnv *env, jobject obj, jbyteArray payment) {

    BRPaymentProtocolRequest *nativeRequest;
    int requestLength = (*env)->GetArrayLength(env, payment);
    jbyte *bytePayment = (*env)->GetByteArrayElements(env, payment, 0);
    if (!TEST_REQ){
        nativeRequest = BRPaymentProtocolRequestParse(bytePayment, requestLength);
    } else {
        nativeRequest = BRPaymentProtocolRequestParse((const uint8_t *) req, sizeof(req) - 1);
    }

    //testing the raw request form the top
    //android debugging
    //__android_log_write(ANDROID_LOG_ERROR, ">>>>>>MESSAGE FROM C: ", nativeRequest->pkiType);

    //read amount and addresses from outputs
    uint64_t outputsLength = nativeRequest->details->outputsCount;
    uint64_t total_amount = 0;
    int singleAddressLength = sizeof(nativeRequest->details->outputs[0].address);
//    char addresses[(singleAddressLength + 2) * outputsLength];

    jobjectArray stringArray;
    stringArray = (jobjectArray) (*env)->NewObjectArray(env, outputsLength,
                                                        (*env)->FindClass(env, "java/lang/String"),
                                                        (*env)->NewStringUTF(env, ""));
    if (outputsLength > 0) {
//         int addressesLength = sizeof(addresses);
        int i;
        for (i = 0; i < outputsLength; i++) {
            BRTxOutput *op = &nativeRequest->details->outputs[i];
            (*env)->SetObjectArrayElement(env, stringArray, i,
                                          (*env)->NewStringUTF(env, op->address));
//            __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Need to print : %s", op->address);
            total_amount += op->amount;
        }
    }

    //signature
    jbyte *bytesSignature = (jbyte *) nativeRequest->signature;
    size_t bytesSignatureSize = nativeRequest->sigLen;
    jbyteArray byteArraySignature = (*env)->NewByteArray(env, bytesSignatureSize);
    (*env)->SetByteArrayRegion(env, byteArraySignature, 0, bytesSignatureSize, bytesSignature);

    //pkiData
    jbyte *bytesPkiData = (jbyte *) nativeRequest->pkiData;
    size_t pkiDataSize = nativeRequest->pkiLen;
    jbyteArray byteArrayPkiData = (*env)->NewByteArray(env, pkiDataSize);
    (*env)->SetByteArrayRegion(env, byteArrayPkiData, 0, pkiDataSize, bytesPkiData);

    //merchantData
    jbyte *bytesMerchantData = (jbyte *) nativeRequest->details->merchantData;
    size_t merchantDataSize = nativeRequest->details->merchDataLen;
    jbyteArray byteArrayMerchantData = (*env)->NewByteArray(env, merchantDataSize);
    (*env)->SetByteArrayRegion(env, byteArrayMerchantData, 0, merchantDataSize, bytesMerchantData);

    //create class
    jclass clazz = (*env)->FindClass(env,
                                     "com/breadwallet/presenter/entities/PaymentRequestCWrapper");
    jobject entity = (*env)->AllocObject(env, clazz);

    //fields
    jfieldID pkiTypeField = (*env)->GetFieldID(env, clazz, "pkiType", "Ljava/lang/String;");
    jfieldID networkField = (*env)->GetFieldID(env, clazz, "network", "Ljava/lang/String;");
    jfieldID timeField = (*env)->GetFieldID(env, clazz, "time", "J");
    jfieldID expiresField = (*env)->GetFieldID(env, clazz, "expires", "J");
    jfieldID memoField = (*env)->GetFieldID(env, clazz, "memo", "Ljava/lang/String;");
    jfieldID paymentURLField = (*env)->GetFieldID(env, clazz, "paymentURL", "Ljava/lang/String;");
    jfieldID addresses = (*env)->GetFieldID(env, clazz, "addresses", "[Ljava/lang/String;");
    jfieldID amount = (*env)->GetFieldID(env, clazz, "amount", "J");

    //methods id
    jmethodID midByteSignature = (*env)->GetMethodID(env, clazz, "byteSignature", "([B)V");
    jmethodID midPkiData = (*env)->GetMethodID(env, clazz, "pkiData", "([B)V");
    jmethodID midMerchantData = (*env)->GetMethodID(env, clazz, "merchantData", "([B)V");

    //set java fields
    (*env)->SetObjectField(env, entity, pkiTypeField,
                           (*env)->NewStringUTF(env, nativeRequest->pkiType));
    (*env)->SetObjectField(env, entity, networkField,
                           (*env)->NewStringUTF(env, nativeRequest->details->network));
    (*env)->SetLongField(env, entity, timeField, nativeRequest->details->time);
    (*env)->SetLongField(env, entity, expiresField, nativeRequest->details->expires);
    (*env)->SetObjectField(env, entity, memoField,
                           (*env)->NewStringUTF(env, nativeRequest->details->memo));
    (*env)->SetObjectField(env, entity, paymentURLField,
                           (*env)->NewStringUTF(env, nativeRequest->details->paymentURL));
    (*env)->SetObjectField(env, entity, addresses, stringArray);
    (*env)->SetLongField(env, entity, amount, total_amount);

    //call java methods
    (*env)->CallVoidMethod(env, entity, midByteSignature, byteArraySignature);
    (*env)->CallVoidMethod(env, entity, midPkiData, byteArrayPkiData);
    (*env)->CallVoidMethod(env, entity, midMerchantData, byteArrayPkiData);

    //release stuff
    (*env)->ReleaseByteArrayElements(env, payment, bytePayment, JNI_COMMIT);
    (*env)->ReleaseByteArrayElements(env, byteArraySignature, bytesSignature, JNI_COMMIT);
    (*env)->ReleaseByteArrayElements(env, byteArrayPkiData, bytesPkiData, JNI_COMMIT);
    (*env)->ReleaseByteArrayElements(env, byteArrayMerchantData, bytesMerchantData, JNI_COMMIT);
    if (nativeRequest) BRPaymentProtocolRequestFree(nativeRequest);

    return entity;
}

JNIEXPORT jbyteArray Java_com_breadwallet_tools_security_RequestHandler_getCertificatesFromPaymentRequest
        (JNIEnv *env, jobject obj, jbyteArray payment, jint index) {

    //create the BRPaymentProtocolRequest
    BRPaymentProtocolRequest *nativeRequest;
    int requestLength = (*env)->GetArrayLength(env, payment);
    jbyte *bytePayment = (*env)->GetByteArrayElements(env, payment, 0);
    if (!TEST_REQ) {
        nativeRequest = BRPaymentProtocolRequestParse(bytePayment, requestLength);
    } else {
        nativeRequest = BRPaymentProtocolRequestParse((const uint8_t *) req, sizeof(req) - 1);
    }
    //testing the raw request example!!!!!!

    //get certificate
    uint8_t *buf[BRPaymentProtocolRequestCert(nativeRequest, NULL, 0, index)];
    size_t length = BRPaymentProtocolRequestCert(nativeRequest, buf, sizeof(buf), index);
//    __android_log_write(ANDROID_LOG_DEBUG, ">>>>>>MESSAGE FROM C: ", (char *) length);

    //convert it to jbyteArray
    jbyte *certJbyte = (const jbyte *) buf;
    jbyteArray result = (*env)->NewByteArray(env, length);
    (*env)->SetByteArrayRegion(env, result, 0, length, certJbyte);
    //release everything
    (*env)->ReleaseByteArrayElements(env, result, certJbyte, JNI_COMMIT);
    (*env)->ReleaseByteArrayElements(env, payment, bytePayment, JNI_COMMIT);
    if (nativeRequest) BRPaymentProtocolRequestFree(nativeRequest);

    return result;
}

JNIEXPORT jboolean JNICALL Java_com_breadwallet_tools_security_RequestHandler_validateAddress
        (JNIEnv *env, jobject obj, jstring address) {
//    jbyte *byte_address = (*env)->GetByteArrayElements(env, address, NULL);
//    jsize size = (*env)->GetArrayLength(env, address);

//    for (int i = 0; i < size; ++i) {
//        printf("bytes[%d] = %x\n", i, ((const unsigned char *) byte_address)[i]);
//        __android_log_print(ANDROID_LOG_DEBUG, "From C:>>>>", charString[i]);
//    __android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "Need to print : %s", charString);
//    }
    jboolean b;
    const char *str;
    str = (char *) (*env)->GetStringUTFChars(env, address, NULL);
//    __android_log_print(ANDROID_LOG_ERROR, "LOG_TAG", "Need to print : %s", str);
    int result = BRAddressIsValid(str);
    (*env)->ReleaseStringUTFChars(env, address, str);
//    __android_log_print(ANDROID_LOG_ERROR, "LOG_TAG", "This is the result : %d", result);
    return result ? JNI_TRUE : JNI_FALSE;

}

JNIEXPORT void JNICALL Java_com_breadwallet_presenter_activities_MainActivity_clearCMemory(JNIEnv *env, jobject obj){
        BRWalletFree(_wallet);
    __android_log_print(ANDROID_LOG_ERROR, "Wallet Freed: ", "BRWalletFree(wallet)");
}

JNIEXPORT void JNICALL Java_com_breadwallet_presenter_activities_MainActivity_cTests(JNIEnv *env, jobject obj){
    __android_log_print(ANDROID_LOG_ERROR, "Core Tests: ", "%d", BRRunTests());
}

//JNIEXPORT void Java_com_breadwallet_presenter_activities_MainActivity_sendMethodCallBack
//        (JNIEnv *env, jobject obj) {
//    jclass cls = (*env)->GetObjectClass(env, obj);
//    jmethodID mid = (*env)->GetMethodID(env, cls, "callback", "()V");
//    if (mid == 0)
//        return;
//    (*env)->CallVoidMethod(env, obj, mid);
//}

//JNIEXPORT jbyteArray Java_com_breadwallet_wallet_BRWalletManager_encodePhrase
//        (JNIEnv *env, jobject obj, jbyteArray seed, jbyteArray wordList) {
//
//    jboolean b;
//    int wordLen = (*env)->GetArrayLength(env, wordList);
//    int seedLen = (*env)->GetArrayLength(env, seed);
//    char *buff[wordLen];
//    char seed_buff[seedLen];
//
//    char *phrase[seedLen]; // wrong ! check later
//
//    jbyte *byte1 = (*env)->GetByteArrayElements(env, wordList, &b);
//    buff = (char *) byte1;
//    (*env)->ReleaseByteArrayElements(env, wordList, b, JNI_COMMIT);
//
//
//    jbyte *seed_byte = (*env)->GetByteArrayElements(env, wordList, &b);
//    seed_buff = seed_byte;
//    (*env)->ReleaseByteArrayElements(env, seed, b, JNI_COMMIT);
//
//    size_t byte_size = BRBIP39Encode(phrase,) continue later
//
//}
