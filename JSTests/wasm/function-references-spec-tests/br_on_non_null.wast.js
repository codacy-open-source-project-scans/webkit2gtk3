// br_on_non_null.wast:1
let $1 = instance("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x91\x80\x80\x80\x00\x03\x60\x00\x01\x7f\x60\x01\x64\x00\x01\x7f\x60\x01\x63\x00\x01\x7f\x03\x88\x80\x80\x80\x00\x07\x01\x02\x00\x00\x00\x00\x00\x07\xbc\x80\x80\x80\x00\x04\x0d\x6e\x75\x6c\x6c\x61\x62\x6c\x65\x2d\x6e\x75\x6c\x6c\x00\x03\x0d\x6e\x6f\x6e\x6e\x75\x6c\x6c\x61\x62\x6c\x65\x2d\x66\x00\x04\x0a\x6e\x75\x6c\x6c\x61\x62\x6c\x65\x2d\x66\x00\x05\x0b\x75\x6e\x72\x65\x61\x63\x68\x61\x62\x6c\x65\x00\x06\x09\x85\x80\x80\x80\x00\x01\x01\x00\x01\x02\x0a\xe6\x80\x80\x80\x00\x07\x8f\x80\x80\x80\x00\x00\x02\x64\x00\x20\x00\xd6\x00\x41\x7f\x0f\x0b\x14\x00\x0b\x8f\x80\x80\x80\x00\x00\x02\x64\x00\x20\x00\xd6\x00\x41\x7f\x0f\x0b\x14\x00\x0b\x84\x80\x80\x80\x00\x00\x41\x07\x0b\x86\x80\x80\x80\x00\x00\xd0\x00\x10\x01\x0b\x86\x80\x80\x80\x00\x00\xd2\x02\x10\x00\x0b\x86\x80\x80\x80\x00\x00\xd2\x02\x10\x01\x0b\x8e\x80\x80\x80\x00\x00\x02\x64\x00\x00\xd6\x00\x41\x7f\x0f\x0b\x14\x00\x0b");

// br_on_non_null.wast:37
assert_trap(() => call($1, "unreachable", []));

// br_on_non_null.wast:39
assert_return(() => call($1, "nullable-null", []), -1);

// br_on_non_null.wast:40
assert_return(() => call($1, "nonnullable-f", []), 7);

// br_on_non_null.wast:41
assert_return(() => call($1, "nullable-f", []), 7);

// br_on_non_null.wast:43
let $2 = instance("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x91\x80\x80\x80\x00\x04\x60\x00\x00\x60\x01\x63\x00\x00\x60\x01\x70\x00\x60\x01\x6f\x00\x03\x84\x80\x80\x80\x00\x03\x01\x02\x03\x0a\xb4\x80\x80\x80\x00\x03\x8c\x80\x80\x80\x00\x00\x02\x64\x00\x20\x00\xd6\x00\x00\x0b\x1a\x0b\x8c\x80\x80\x80\x00\x00\x02\x64\x70\x20\x00\xd6\x00\x00\x0b\x1a\x0b\x8c\x80\x80\x80\x00\x00\x02\x64\x6f\x20\x00\xd6\x00\x00\x0b\x1a\x0b");

// br_on_non_null.wast:51
let $3 = instance("\x00\x61\x73\x6d\x01\x00\x00\x00\x01\x93\x80\x80\x80\x00\x03\x60\x01\x7f\x01\x7f\x60\x02\x7f\x63\x00\x01\x7f\x60\x00\x02\x7f\x64\x00\x03\x85\x80\x80\x80\x00\x04\x00\x01\x00\x00\x07\x96\x80\x80\x80\x00\x02\x09\x61\x72\x67\x73\x2d\x6e\x75\x6c\x6c\x00\x02\x06\x61\x72\x67\x73\x2d\x66\x00\x03\x09\x85\x80\x80\x80\x00\x01\x01\x00\x01\x00\x0a\xba\x80\x80\x80\x00\x04\x87\x80\x80\x80\x00\x00\x20\x00\x20\x00\x6c\x0b\x8e\x80\x80\x80\x00\x00\x02\x02\x20\x00\x20\x01\xd6\x00\x0f\x0b\x14\x00\x0b\x88\x80\x80\x80\x00\x00\x20\x00\xd0\x00\x10\x01\x0b\x88\x80\x80\x80\x00\x00\x20\x00\xd2\x00\x10\x01\x0b");

// br_on_non_null.wast:72
assert_return(() => call($3, "args-null", [3]), 3);

// br_on_non_null.wast:73
assert_return(() => call($3, "args-f", [3]), 9);
