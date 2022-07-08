
import { Component } from "react";
import { GiSaveArrow } from "react-icons/gi"

class FileDownload extends Component {
    //const FileDownload = (props) => {

    handleClick = () => {
        !(this.props.dataFromParent.endsWith(" downloaded") || this.props.dataFromParent.endsWith("No File Uploaded") )  ?
            this.props.parentCallBack(null, this.props.dataFromParent + " downloaded")
            : this.props.parentCallBack(null, this.props.dataFromParent);
    }

    
    render() {

        var display =  this.props.dataFromParent.endsWith("No File Uploaded") 
        ? "inactiveFileButton"
        : "uploadButton";

        return (
            <div>
                <button
                    id = {display}
                    onClick={this.handleClick}
                >
                    Download File <GiSaveArrow className="icon pull-right" />

                </button>
            </div>
        );
    }
}

export default FileDownload;
