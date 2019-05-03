"use strict";
// Copyright © 2019 The Things Network Foundation, The Things Industries B.V.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
var App_1 = __importDefault(require("./App"));
var server = process.env.TTN_SERVER || "http://localhost:1885";
var appID = process.env.TTN_APPLICATION_ID;
var apiKey = process.env.TTN_API_KEY;
var devID = process.env.TTN_DEVICE_ID;
if (appID == undefined) {
    throw new Error("TTN_APPLICATION_ID is unset");
}
if (apiKey == undefined) {
    throw new Error("TTN_API_KEY is unset");
}
if (devID == undefined) {
    throw new Error("TTN_DEVICE_ID is unset");
}
var app = new App_1.default(server, appID, apiKey);
app.syncClock(devID).catch(function (err) {
    console.log("sync failed:", err.message);
});
//# sourceMappingURL=index.js.map