// Copyright (C) 2022 Igalia, S.L. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
esid: sec-temporal.timezone.prototype.getpossibleinstantsfor
description: Leap second is a valid ISO string for a calendar in a property bag
includes: [compareArray.js]
features: [Temporal]
---*/

const instance = new Temporal.TimeZone("UTC");

const calendar = "2016-12-31T23:59:60";

const arg = { year: 1976, monthCode: "M11", day: 18, calendar };
const result = instance.getPossibleInstantsFor(arg);
assert.compareArray(
  result.map(i => i.epochNanoseconds),
  [217_123_200_000_000_000n],
  "leap second is a valid ISO string for calendar"
);
