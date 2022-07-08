import React, { Component } from "react";

import ExpandCollapseButton from "../ExpandCollapseButton";
import DiscussionTable from "../DiscussionTable/DiscussionTable";

import "../../Styles/DiscussionTableManager.css";

const TITLE = "Discussion Tables";

export default class DiscussionManager extends Component {
	constructor(props) {
		super(props);
		//setting defaults
		this.state = {
			tables: [],
		};
		if (props.tables) {
			//allows default tables in constructor
			this.state.tables = props.tables;
		}
	}

	toggleTables() {
		//expand/ collapse tables
		this.setState({
			showTables: !this.state.showTables,
		});
	}

	createTable(id, name) {
		this.setState({
			tables: [
				...this.state.tables,
				{ id: id, name: name, participants: [] },
			],
		});
	}

	getHighestId() {
		//get highest ID in all tables
		return this.state.tables.reduce((a, x) => Math.max(a, x.id), 0);
	}

	addTable() {
		//allows user to create new table
		const name = prompt("Please enter a name: ");
		this.createTable(this.getHighestId() + 1, name);
	}

	renderTable(i) {
		return (
			<DiscussionTable
				id={this.state.tables[i].id}
				participants={this.state.tables[i].participants}
				name={this.state.tables[i].name}
			></DiscussionTable>
		);
	}

	renderTables() {
		let result = [];
		for (let i = 0; i < this.state.tables.length; i++) {
			result.push(this.renderTable(i));
		}
		return result;
	}

	/* 

      */
	render() {
		return (
			<div className="DiscussionManager">
				<div className="DiscussionManagerHeader">
					<h5 className="DiscussionManagerTitle">{TITLE}</h5>
					<button
						className="DiscussionManagerAddButton"
						onClick={this.addTable.bind(this)}
					>
						Add Table
					</button>

					<ExpandCollapseButton
						className="ToggleParticipants"
						fillUp={"#212529"}
						fillDown={"#212529"}
						onClick={this.toggleTables.bind(this)}
					>
						Open/close
					</ExpandCollapseButton>
				</div>

				<div className="DiscussionManagerTables">
					<div
						id="DiscussionManagerTables"
						className={
							this.state.showTables ? "expanded" : "collapsed"
						}
					>
						{this.renderTables()}
					</div>
				</div>
			</div>
		);
	}
}
