import React, { Component } from "react";

import "../Styles/ParticipantHeader.css";

export default class ParticipantHeader extends Component {
	constructor(props) {
		super(props);
		//setting defaults
		this.state = {
			participant: null,
		};

		//participant from properties
		if (props.participant) {
			this.state.participant = props.participant;
		}
        else {
            throw new Error("Must have participant in props...")
        }
	}

	/* 

      */
	render() {
		return (
			<div className="ParticipantHeader">
				<p
					className="ParticipantHeaderName"
					style={{ marginBottom: "0px" }}
				>
					{this.state.participant.name}
				</p>
			</div>
		);
	}
}
