import { Link } from 'react-router-dom';
import { HiOutlineUserGroup } from "react-icons/hi";
import Clock from './Clock';
import '../Styles/Header.css'

const Header = ( { type } ) => {
  return (
    <div className="header">
      <ul className="headerContainer">
        <li><HiOutlineUserGroup className="icon" /> 0</li>
        <li><Clock /></li>
        <li><Link to="/"><button id="leaveButton">Leave {type}</button></Link></li>
      </ul>
    </div>
  );
}

export default Header;
