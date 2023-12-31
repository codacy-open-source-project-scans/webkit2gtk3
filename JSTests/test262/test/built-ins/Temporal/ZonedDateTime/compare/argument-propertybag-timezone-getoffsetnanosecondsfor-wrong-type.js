// Copyright (C) 2021 Igalia, S.L. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
esid: sec-temporal.zoneddatetime.compare
description: TypeError thrown if time zone reports an offset that is not a Number
features: [Temporal]
includes: [temporalHelpers.js]
---*/

[
  undefined,
  null,
  true,
  "+01:00",
  Symbol(),
  3600_000_000_000n,
  {},
  {
    valueOf() {
      return 3600_000_000_000;
    }
  }
].forEach((wrongOffset) => {
  const timeZone = TemporalHelpers.specificOffsetTimeZone(wrongOffset);
  const datetime = new Temporal.ZonedDateTime(1_000_000_000_987_654_321n, timeZone);

  assert.throws(
    TypeError,
    () => Temporal.ZonedDateTime.compare({ year: 2000, month: 5, day: 2, hour: 12, timeZone }, datetime),
    `invalid offset: ${String(wrongOffset)} (${typeof wrongOffset})`
  );
  assert.throws(
    TypeError,
    () => Temporal.ZonedDateTime.compare(datetime, { year: 2000, month: 5, day: 2, hour: 12, timeZone }),
    `invalid offset: ${String(wrongOffset)} (${typeof wrongOffset})`
  );
});
