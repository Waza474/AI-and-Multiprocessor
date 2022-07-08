import React, {Component} from 'react';
import {FiVideo, FiVideoOff} from "react-icons/fi";

class VideoButton extends Component{
    constructor(props){
        super(props);
            this.state={
                isOn:false
            }
        }
        videoButtonHandler= ()=>{
            this.setState(state =>({
              isOn : !state.isOn
            }));
          }
          render (){
            return (
                <div>
                    <button
                        id = "videoAudioPanel"
                        onClick ={this.videoButtonHandler}>
                        Video  {this.state.isOn? <FiVideo/>:<FiVideoOff/>}
                    </button>

                </div>
            )
            
        
        }
    }
export default VideoButton;