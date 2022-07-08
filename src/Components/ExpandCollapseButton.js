import React, { Component } from "react";
import { AiFillDownSquare, AiFillUpSquare } from "react-icons/ai";

import "../Styles/ExpandCollapseButton.css";

class ExpandCollapseButton extends Component {
	constructor(props) {
		super(props);
		this.state = {
			isCollapsed: false, //whether or not the button is collapsed
			fillDown: "rgb(24 91 164)",
			fillUp: "rgb(0 152 189)",
		};
		if (this.props.fillUp) {
			this.state.fillUp = this.props.fillUp
		}
		if (this.props.fillDown) {
			this.state.fillDown = this.props.fillDown;
		}
	}
	toggle = () => {
		//allows onClick function to be passed to button
		if (typeof this.props.onClick === "function") {
			this.props.onClick();
		}

		//toggles the icon
		this.setState((state) => ({
			isCollapsed: !state.isCollapsed,
		}));
	};
	render() {
		return (
			<div>
				<button
					className={
						this.state.isCollapsed ? " Collapsed" : "Expanded"
					}
					id="ExpandCollapseButton"
					onClick={this.toggle}
				>
					{this.state.isCollapsed ? (
						<AiFillDownSquare
							className="Icon"
							size={45}
							style={{ fill: this.state.fillDown }}
						/>
					) : (
						<AiFillUpSquare
							className="Icon"
							size={45}
							style={{ fill: this.state.fillUp }}
						/>
					)}
				</button>
			</div>
		);
	}
}
export default ExpandCollapseButton;
