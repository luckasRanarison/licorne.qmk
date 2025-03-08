import St from "gi://St";
import Clutter from "gi://Clutter";
import GObject from "gi://GObject";

import * as PanelMenu from "resource:///org/gnome/shell/ui/panelMenu.js";
import { LAYERS } from "../utils/constants.js";

class _Indicator extends PanelMenu.Button {
  _init() {
    super._init(1, "Licorne indicator");

    this._layer = new St.Label({ text: "" });
    this._speedometer = new St.Label({ text: "", styleClass: "speedometer" });
    this._wpm = new St.Label({ text: "" });

    const container = new St.BoxLayout({
      y_align: Clutter.ActorAlign.CENTER,
      styleClass: "indicator-container",
    });

    container.add_child(this._layer);
    container.add_child(this._speedometer);
    container.add_child(this._wpm);

    this.add_child(container);
  }

  /**
   * @param params {{layer?: string, wpm?: string}}
   */
  update({ layer, wpm }) {
    if (wpm) {
      const speedometer = this._makeSpeedometer(parseInt(wpm));
      this._speedometer?.set_text(speedometer);
      this._wpm?.set_text(`${wpm} WPM`);
    }

    if (layer) {
      if (!LAYERS.has(layer)) {
        console.error("Unknown layer: ", layer);
      }

      this._layer?.set_text(`-- ${layer} --`);
    }
  }

  /**
   * @param wpm {number}
   */
  _makeSpeedometer(wpm) {
    const scale = 7;
    const maxWpm = 255;

    const progress = Math.min(Math.max(wpm, 0), maxWpm);
    const step = Math.round((progress / maxWpm) * scale);

    let graduatedBar = "";

    for (let i = 0; i < scale; i++) {
      if (i < step) {
        graduatedBar += ">>";
      } else {
        graduatedBar += "--";
      }
    }

    return `[${graduatedBar.padEnd(scale, "—")}]`;
  }
}

const Indicator = GObject.registerClass(_Indicator);

export default Indicator;
