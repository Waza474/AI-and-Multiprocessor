import React, {Component} from 'react';
import {BiMicrophone, BiMicrophoneOff} from "react-icons/bi";

class AudioButton extends Component{
    constructor(props){
        super(props);
            this.state={
                muted:false
            }
        }
        muteButtonHandler= ()=>{
            this.setState(state =>({
              muted : !state.muted
            }));
          }
          render (){
            return (
                <div>
                    <button
                        id = "videoAudioPanel"
                        onClick ={this.muteButtonHandler}>
                        Mic  {this.state.muted ? <BiMicrophone/>:<BiMicrophoneOff/>}
                    </button>

                </div>
            )
            
        
        }
    }
export default AudioButton;