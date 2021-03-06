# XR Browser Tests

## Introduction

This documentation concerns `xr_browser_test.h`, `xr_browser_test.cc`, and files
that use them or their subclasses.

These files port the framework used by XR instrumentation tests (located in
[`//chrome/android/javatests/src/org/chromium/chrome/browser/vr/`][1] and
documented in
`//chrome/android/javatests/src/org/chromium/chrome/browser/vr/*.md`) for
use in browser tests in order to test XR features on desktop platforms.

This is pretty much a direct port, with the same JavaScript/HTML files being
used for both and the Java/C++ code being functionally equivalent to each other,
so the instrumentation test's documentation on writing tests using the framework
is applicable here, too. As such, this documentation only covers any notable
differences between the two implementations.

## Restrictions

Both the instrumentation tests and browser tests have hardware/software
restrictions - in the case of browser tests, XR is only supported on Windows 8
and later (or Windows 7 with a non-standard patch applied) with a GPU that
supports DirectX 11.1.

Instrumentation tests handle restrictions with the `@Restriction` annotation,
but browser tests don't have any equivalent functionality. However, since
the browser tests don't require any setup outside of the tests like
instrumentation tests do (which require the paired viewer, etc. to be changed),
having the tests in their own target is sufficient to prevent us from running
into hardware incompatibility issues.

## Command Line Switches

Instrumentation tests are able to add and remove command line switches on a
per-test-case basis using `@CommandLine` annotations, but equivalent
functionality does not exist in browser tests.

Instead, if different command line flags are needed, a new class will need to
be created that extends the correct type of `*BrowserTestBase` and overrides the
flags that are set in its `SetUp` function.

## Compiling And Running

The tests are compiled in the `xr_browser_tests` target. This is a combination
of the `xr_browser_tests_binary` target, which is the actual test, and the
`xr_browser_tests_runner` target, which is a wrapper script that ensures special
setup is completed before running the tests.

Once compiled, the tests can be run using the following command line:

`run_xr_browser_tests.py --enable-gpu --test-launcher-jobs=1
--enable-pixel-output-in-tests`

Because the "test" is actually a Python wrapper script, you may need to prepend
`python` to the front of the command on Windows if Python file association is
not set up on your machine.

[1]: https://chromium.googlesource.com/chromium/src/+/master/chrome/android/javatests/src/org/chromium/chrome/browser/vr