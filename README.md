# tpm2-utils

There are surprisingly little utilities out there for interacting with TPM 2.0 
devices. This repository currently holds a single program, but more might be 
added in the future.

You'll probably need Linux 4.0 or higher.

See also [Linux Kernel Bug 98181](https://bugzilla.kernel.org/show_bug.cgi?id=98181)

## tpm2-test

A simple tool with which you can test whether your TPM 2.0 device is 
functioning. It will output the raw response to a couple of 
`TPM_CC_GetCapability` requests.
