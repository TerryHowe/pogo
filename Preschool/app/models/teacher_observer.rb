class TeacherObserver < ActiveRecord::Observer
  def before_save(teacher)
    return false if teacher.room == nil
    return false if teacher.photo == nil
  end
end
