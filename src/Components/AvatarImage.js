import React, { Component } from 'react';
import avatarDefault from '../Images/AvatarDefault.png';

import '../Styles/AvatarImage.css' 


const DEFAULT_WIDTH = 100;
const DEFAULT_HEIGHT = 100;

const USER_WIDTH = 300;
const USER_HEIGHT = 300;

export default class AvatarImage extends Component {


    constructor(props) {
        super(props);
        //setting defaults
        this.state = {
            width: DEFAULT_WIDTH,
            height: DEFAULT_HEIGHT,
            image: avatarDefault,
            text: "",
          };

        //allow props.user = true to indicate that this is the client's avatar.
        if (props.user) { 
            this.state.width = USER_WIDTH;
            this.state.height = USER_HEIGHT;
        }
        //allow props.width and props.height values
        if (props.width) {
            this.state.width = props.width;
        }
        if (props.height) {
            this.state.height = props.height;
        }
        
        //using image from properties if possible
        if (props.image) {
            this.state.image = props.image;
        }

        //text from properties
        if (props.text) {
            this.state.text = props.text;
        }
       
      }

      /* 

      */
  render() {
    return (
        <div className="AvatarImage">
            <p className="AvatarImageTitle"             
            style={{marginBottom: "0px"}}
            > 
                {this.state.text} 
            </p>
          <img src={this.state.image} width={this.state.width} height={this.state.height} alt="avatar" 
            style={{marginLeft: "0px"}}
          />
        </div>
    );
  }
} 