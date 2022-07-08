import { Component } from "react";
//import { useRef } from 'react';
import { FaLock } from "react-icons/fa";

//const hiddenFileInput = useRef(null);

class FileUpload extends Component {
    //const FileUpload = (props) => {


    // When the user clicks the big button, it refers to the hidden buttons onclick functionality
    handleClick = () => {
        this.refs.hiddenFileInput.click()
    };

    handleChange = (event) => {
        const fileUploaded = event.target.files[0];
        this.props.parentCallBack(fileUploaded, fileUploaded.name);
        //this.props.parentCallBack(event.target.files[0].name);
    };

    render() {
        return (
            <div>
                <button
                    onClick={this.handleClick}
                    id="uploadButton">
                    Share File <FaLock className="icon pull-right" />
                </button>
                <input
                    type="file"
                    ref="hiddenFileInput"
                    onChange={this.handleChange}
                    hidden />
            </div>
        );
    }
}

export default FileUpload;
