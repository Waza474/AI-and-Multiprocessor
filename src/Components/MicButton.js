import { Component } from "react";
import { FaMicrophone, FaMicrophoneSlash } from "react-icons/fa"

class MicButton extends Component {
    
    constructor(props) {
        super(props)
        this.state = { icon: FaMicrophoneSlash }

    }

    handleClick = () => {
        this.state.icon == FaMicrophoneSlash ? this.setState({icon: FaMicrophone}) : this.setState({icon: FaMicrophoneSlash}) 
    }

    render() {
        return (
            <div>
                <button
                    id="audioButton"
                    onClick = {this.handleClick}
                >
                    <this.state.icon/>
                </button>
            </div>
        )
    }
}

export default MicButton;