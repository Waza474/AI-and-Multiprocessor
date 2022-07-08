import Header from '../Components/Header';
import FileUpload from '../Components/FileUpload';

import Chat from '../Components/Chat';
import AvatarImage from '../Components/AvatarImage';
import OthersAvatars from '../Components/OthersAvatars';
import FileDownload from '../Components/FileDownload';
import AudioButton from '../Components/Audio';
import VideoButton from '../Components/Video';
import { Component } from 'react';
import QA from '../Components/QA';

import RegisteredUser from '../Classes/Users/RegisteredUser';

import '../Styles/Lecture.css' 


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

var participants = [ //for participants/ avatars testing
  new RegisteredUser(1, "Dave", false), 
  new RegisteredUser(2, "Leon", false),
  new RegisteredUser(1, "Daniel", false), 
  new RegisteredUser(2, "Pavle", false),
]

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
];

class Lecture extends Component {
  constructor(props) {
    super(props);
    this.state = { 
      fileName: "No File Uploaded",
      participants: participants2, //list of participants such as students/ teachers etc. from domain model classes
      user: participants[0] //the current user i.e whoever is running the client.
  
  }
  //allow the current user to be passed as a property to the lecture
  if (this.props.user) {
    this.addParticipant(this.props.user);
    this.state.user = this.props.user;
  }

  }

  handleFile = (childFile, childString) => {
    this.setState({ file: childFile})
    this.setState({ fileName: childString })
    console.log(this.state);
  }

  addParticipant = (participant) => {
    this.state.participants.push(participant);
  }

  render() {
    //const Lecture = () => {
    return (
      <div>
        <Header type="Lecture" />
        <div className="content">
          <div>
            <AvatarImage user={true} text={"Andreas"}/>
            <FileUpload parentCallBack={this.handleFile} />
            <FileDownload parentCallBack={this.handleFile}
              dataFromParent = {this.state.fileName} />
              <div>
              <AudioButton/> 
              <VideoButton/>
            </div>
            <p>{this.state.fileName}</p>
          </div>
          <div className="col2">
            <img src="https://via.placeholder.com/500x400" alt="slideshow" />
          </div>
          <div className="col3">
            <QA />
            <Chat
              messages={messages2}
              user={user}
            />
          </div>
            <OthersAvatars
              participants = {this.state.participants}
            />

        </div>
      </div>
    );
  }
}

export default Lecture;
