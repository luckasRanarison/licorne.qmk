import St from "gi://St";
import Clutter from "gi://Clutter";
import GObject from "gi://GObject";

import * as PanelMenu from "resource:///org/gnome/shell/ui/panelMenu.js";

class _Indicator extends PanelMenu.Button {
  _init() {
    super._init(1, "Licorne indicator");
    this._label = new St.Label({ text: "BASE" });
    this.set_y_align(Clutter.ActorAlign.CENTER);
    this.add_child(this._label);
  }

  /**
   * @param text {string}
   */
  upateText(text) {
    this._label?.set_text(text);
  }
}

const Indicator = GObject.registerClass(_Indicator);

export default Indicator;
