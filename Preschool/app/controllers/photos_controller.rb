class PhotosController < ApplicationController
  def index
    @photos = Photo.all

    respond_to do |format|
      format.html # index.html.erb
      format.json { render :json => @photos }
    end
  end

  def show
    @photo = Photo.find(params[:id])

    respond_to do |format|
      format.html # show.html.erb
      format.json { render :json => @photo }
    end
  end

  def new
    @photo = Photo.new

    respond_to do |format|
      format.html # new.html.erb
      format.json { render :json => @photo }
    end
  end

  def edit
    @photo = Photo.find(params[:id])
    if (params[:teacher_id] != nil)
      @teacher = Teacher.find(params[:teacher_id])
      session[:teacher_id] = @teacher.id
    end
  end

  def create
    @photo = Photo.new(params[:photo])
    @photo.image = File.new(upload_path)
    @photo.save

    respond_to do |format|
      if @photo.save
        format.html { redirect_to @photo, :notice => 'Photo was successfully created.' }
        format.json { render :json => @photo, :status => :created, :location => @photo }
      else
        format.html { render :action => "new" }
        format.json { render :json => @photo.errors, :status => :unprocessable_entity }
      end
    end
  end

  def update
    puts '*********************************************'
    puts '****** ' + params[:id].to_s
    puts '****** ' + params[:return_path].to_s
    puts '*********************************************'
    @photo = Photo.find(params[:id])
    @photo.image = File.new(upload_path)
    @photo.save

    respond_to do |format|
      if @photo.update_attributes(params[:photo])
        format.html { redirect_to @photo, :notice => 'Photo was successfully updated.' }
        format.json { head :ok }
      else
        format.html { render :action => "edit" }
        format.json { render :json => @photo.errors, :status => :unprocessable_entity }
      end
    end
  end

  def destroy
    @photo = Photo.find(params[:id])
    @photo.destroy

    respond_to do |format|
      format.html { redirect_to photos_url }
      format.json { head :ok }
    end
  end

  def upload
    File.open(upload_path, 'w') do |f|
      f.write request.raw_post
    end
    render :text => "ok"
  end

private

  def upload_path
    file_name = session[:session_id].to_s + '.jpg'
    File.join(Rails.root, 'public', 'uploads', file_name)
  end
end
