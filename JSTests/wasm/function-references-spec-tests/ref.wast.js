// ref.wast:3
let $1 = instance("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x9b\x80\x80\x80\x00\x02\x60\x00\x00\x60\x0c\x70\x6f\x64\x70\x64\x6f\x64\x00\x64\x00\x64\x00\x64\x00\x70\x6f\x63\x00\x63\x00\x00\x03\x82\x80\x80\x80\x00\x01\x01\x0a\x88\x80\x80\x80\x00\x01\x82\x80\x80\x80\x00\x00\x0b");

// ref.wast:27
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x86\x80\x80\x80\x00\x01\x60\x01\x64\x01\x00");

// ref.wast:31
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x86\x80\x80\x80\x00\x01\x60\x00\x01\x64\x01");

// ref.wast:36
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x06\x87\x80\x80\x80\x00\x01\x63\x01\x00\xd0\x01\x0b");

// ref.wast:41
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x04\x85\x80\x80\x80\x00\x01\x63\x01\x00\x0a");

// ref.wast:46
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x09\x85\x80\x80\x80\x00\x01\x05\x64\x01\x00");

// ref.wast:51
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x86\x80\x80\x80\x00\x01\x60\x01\x64\x01\x00\x03\x82\x80\x80\x80\x00\x01\x00\x0a\x88\x80\x80\x80\x00\x01\x82\x80\x80\x80\x00\x00\x0b");

// ref.wast:55
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x86\x80\x80\x80\x00\x01\x60\x00\x01\x64\x01\x03\x82\x80\x80\x80\x00\x01\x00\x0a\x88\x80\x80\x80\x00\x01\x82\x80\x80\x80\x00\x00\x0b");

// ref.wast:59
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x84\x80\x80\x80\x00\x01\x60\x00\x00\x03\x82\x80\x80\x80\x00\x01\x00\x0a\x8b\x80\x80\x80\x00\x01\x85\x80\x80\x80\x00\x01\x01\x63\x01\x0b");

// ref.wast:64
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x84\x80\x80\x80\x00\x01\x60\x00\x00\x03\x82\x80\x80\x80\x00\x01\x00\x0a\x8e\x80\x80\x80\x00\x01\x88\x80\x80\x80\x00\x00\x02\x64\x01\x00\x0b\x1a\x0b");

// ref.wast:68
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x84\x80\x80\x80\x00\x01\x60\x00\x00\x03\x82\x80\x80\x80\x00\x01\x00\x0a\x8e\x80\x80\x80\x00\x01\x88\x80\x80\x80\x00\x00\x03\x64\x01\x00\x0b\x1a\x0b");

// ref.wast:72
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x84\x80\x80\x80\x00\x01\x60\x00\x00\x03\x82\x80\x80\x80\x00\x01\x00\x0a\x8d\x80\x80\x80\x00\x01\x87\x80\x80\x80\x00\x00\x04\x64\x01\x0b\x1a\x0b");

// ref.wast:77
assert_invalid("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x84\x80\x80\x80\x00\x01\x60\x00\x00\x03\x82\x80\x80\x80\x00\x01\x00\x0a\x8e\x80\x80\x80\x00\x01\x88\x80\x80\x80\x00\x00\x00\x1c\x01\x64\x01\x1a\x0b");
