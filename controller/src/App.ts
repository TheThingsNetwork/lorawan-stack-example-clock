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

import axios from "axios"
import DownlinkMessage from "./DownlinkMessage"

class App {
  private static defaultDownlinkMessage: DownlinkMessage = {
    "f_port": 15,
    priority: "NORMAL",
  }

  public server: string
  public appID: string
  public apiKey: string

  public constructor(server: string, appID: string, apiKey: string) {
    this.server = server
    this.appID = appID
    this.apiKey = apiKey
  }

  private async replaceQueue(devID: string, items: DownlinkMessage[]): Promise<void> {
    let url = `${this.server}/api/v3/as/applications/${this.appID}/devices/${devID}/down/replace`
    let data = {
      downlinks: items,
    }
    try {
      await axios.post(url, data, {
        headers: {
          "Authorization": `Bearer ${this.apiKey}`,
        },
      })
    } catch (e) {
      console.log("request failed:", e.response.data)
      throw e
    }
  }

  public async syncClock(devID: string): Promise<void> {
    let now = new Date()
    let target = new Date(now.getFullYear(), now.getMonth(), now.getDate(), now.getHours(),
      now.getMinutes() + (now.getSeconds() <= 50 ? 1 : 2),
    )
    console.log(`sync clock ${devID}: now = ${now.toJSON()}; target = ${target.toJSON()}`)

    let buffer = Buffer.allocUnsafe(4)
    buffer.writeUInt32LE(target.getTime() / 1000, 0)

    await this.replaceQueue(devID, [
      {
        ...App.defaultDownlinkMessage,
        "frm_payload": Array.from(buffer),
        "class_b_c": {
          "absolute_time": target,
        },
      },
    ])
  }
}

export default App
