import Header from "../Components/Header";
import DiscussionTable from "../Components/DiscussionTable/DiscussionTable";
import DiscussionManager from "../Components/DiscussionTable/DiscussionManager";

import Chat from '../Components/Chat';
import AvatarImage from '../Components/AvatarImage';

import RegisteredUser from "../Classes/Users/RegisteredUser";

import "../Styles/Classroom.css";

import 'bootstrap/dist/css/bootstrap.min.css';
import { Container, Row, Col } from 'react-bootstrap';

import AudioButton from "../Components/Audio";
import VideoButton from "../Components/Video";
import FileDownload from '../Components/FileDownload';
import FileUpload from '../Components/FileUpload';
import { Component } from 'react';
import OthersAvatars from '../Components/OthersAvatars';
import '../Styles/Classroom.css'

var participants = [
	//for participants/ avatars testing
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),
];

var participants2 = [
	//for participants/ avatars testing
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),
	new RegisteredUser(1, "Dave", false),
	new RegisteredUser(2, "Leon", false),
	new RegisteredUser(1, "Daniel", false),
	new RegisteredUser(2, "Pavle", false),

];

//Testing constants
const user = { //for Messaging library
	"name": "me",
	"uid": "user1",
	"avatar": "https://data.cometchat.com/assets/images/avatars/spiderman.png",
};

const messages2 = [ //for messaging library
	{
		"text": "Hello there",
		"id": "1",
		"sender": {
			"name": "Ironman",
			"uid": "user2",
			"avatar": "https://data.cometchat.com/assets/images/avatars/ironman.png",
		},
	},
]

class Classroom extends Component {

	constructor(props) {
		super(props);
		this.state = {
			fileName: "No File Uploaded",
			participants: participants2, //list of participants such as students/ teachers etc. from domain model classes
			user: participants2[0] //the current user i.e whoever is running the client.
		}
		//allow the current user to be passed as a property to the lecture
		if (this.props.user) {
			this.addParticipant(this.props.user);
			this.state.user = this.props.user;
		}

	}

	handleFile = (childFile, childString) => {
		this.setState({ file: childFile })
		this.setState({ fileName: childString })
		console.log(this.state);
	}

	addParticipant = (participant) => {
		this.state.participants.push(participant);
	}

	render() {
		return (
			<div>
				<Header type="Classroom" />
				<Container fluid>
					<Row>
						<h1 className="hiddenfont">⠀</h1>
					</Row>
					<Row>
						<Col>
							<Row>
								<Col md="auto">
									<img src="https://via.placeholder.com/500x330" alt="slideshow" />
								</Col>
								<Col md="auto">
									<AvatarImage user={true} text={"Teacher"} />
								</Col>
								<Col md="auto">
									<DiscussionManager
									tables = {[
										{id: 1, name: "Epic Room", participants: participants},
										{id: 23, name: "Super Epic Room!", participants: []}
											]}
									></DiscussionManager>
									
								</Col>
							</Row>
							<Row>
								<h1 className="hiddenfont">⠀</h1>
							</Row>
							<Row>
								<Col></Col>
								<Col md="auto">
									<OthersAvatars
										participants={this.state.participants}
									/>
								</Col>
								<Col></Col>
							</Row>
						</Col>
						<Col md="auto">
							<Chat
								messages={messages2}
								user={user}
							/>
						</Col>
					</Row>
					<Row>
						<h1 className="hiddenfont">⠀</h1>
					</Row>
					<Row>
						<Col md="auto">
							<Row>
								<FileUpload parentCallBack={this.handleFile} />
							</Row>
							<Row>
								<FileDownload parentCallBack={this.handleFile}
									dataFromParent={this.state.fileName} />
							</Row>
						</Col>
						<Col>
						</Col>
						<Col md="auto">
							<Col>
								<VideoButton />
							</Col>
							<Col>
								<AudioButton />
							</Col>
						</Col>
					</Row>
					<Row>
						<h1 className="hiddenfont">⠀</h1>
					</Row>
				</Container>
			</div>
		);
	}
}
export default Classroom;
