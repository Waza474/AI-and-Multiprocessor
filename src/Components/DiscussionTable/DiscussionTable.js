import React, { Component } from "react";

import ExpandCollapseButton from "../ExpandCollapseButton";
import ParticipantHeader from "../ParticipantHeader";

import "../../Styles/DiscussionTable.css";

export default class DiscussionTable extends Component {
	constructor(props) {
		super(props);
		//setting defaults
		this.state = {
			showParticipants: false, //whether or not we have expanded the participants
			id: 0, //room ID, should be unique to each room, required
			name: "", //Optional name of table
			participants: [], //list of participants inside table
		};
		if (props.id) {
			this.state.id = props.id;
		} else {
			throw new Error("ID required in initialization...");
		}

		//allow participants in initialization
		if (props.participants) {
			this.state.participants = props.participants;
		}

		//allow name in initialization
		if (props.name) {
			this.state.name = props.name;
		}
	}

	toggleParticipants() {
		this.setState({
			showParticipants: !this.state.showParticipants,
		});
	}

	renderParticipant(i) {
		return (
			<ParticipantHeader
				participant={this.state.participants[i]}
			></ParticipantHeader>
		);
	}

	renderParticipants() {
		let result = [];
		for (let i = 0; i < this.state.participants.length; i++) {
			result.push(this.renderParticipant(i));
		}
		return result;
	}

	/* 

      */
	render() {
		return (
			<div className="DiscussionTable">
				<div className="DiscussionTableRow1">
					<span className="DiscussionTableId">{this.state.id}</span>
					<p className="DiscussionTableTitle">{this.state.name}</p>
					<button
						className="DiscussionTableJoinButton"
						onClick={function () {
							console.log("Joined!");
						}}
					>
						Join
					</button>
					<ExpandCollapseButton
						className="ToggleParticipants"
						onClick={this.toggleParticipants.bind(this)}
					>
						Open/close
					</ExpandCollapseButton>
				</div>

				<div className="DiscussionTableRow2">
					<div
						id="DiscussionTableParticipants"
						className={
							this.state.showParticipants
								? "expanded"
								: "collapsed"
						}
					>
						{this.renderParticipants()}
					</div>
				</div>
			</div>
		);
	}
}
