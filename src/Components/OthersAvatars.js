import React, { Component } from 'react';
import AvatarImage from '../Components/AvatarImage';
import '../Styles/OthersAvatars.css' 

export default class OthersAvatars extends Component {


    constructor(props) {
        super(props);
        
        this.state = {
            participants: props.participants,

          };        
       
    }

    renderAvatar(i) {
      return (
        <AvatarImage 
        user={false} 
        text={this.state.participants[i].name}
        />
        );
    }

    renderAvatars() {
      let result = []
      for (let i = 0; i < this.state.participants.length; i++) {
        result.push(this.renderAvatar(i));
      }
      return result;
    }

    render() {
      return (
          <div className="OthersAvatars">
            {this.renderAvatars()}
          </div>
      );
    }
  } 