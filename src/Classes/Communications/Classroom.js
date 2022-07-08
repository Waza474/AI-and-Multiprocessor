class Classroom {
    constructor(subject) {
        this.subject = subject;
        this.students = [];
        this.teachers = [];
    }

    getCount() {
        return this.students.length + this.teachers.length;
    }

}