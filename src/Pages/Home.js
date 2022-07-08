import { Link } from 'react-router-dom';

const Home = () => {
  return (
    <div>
      <h1>HOME</h1>
      <Link to="/">Home</Link><br />
      <Link to="/lecture">Lecture</Link><br />
      <Link to="/classroom">Classroom</Link><br />
      <Link to ="/login">Log In Page</Link>
    </div>
  );
}

export default Home;
